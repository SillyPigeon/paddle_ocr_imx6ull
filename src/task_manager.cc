// Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "task_manager.h"

#define MAX_CAPTURE_SIZE        10

using namespace paddle::lite_api; // NOLINT
using namespace std;

std::chrono::time_point<std::chrono::system_clock> start_init_time, end_init, \
                                                   start_ocr_time, end_load_img, end_RunDetModel, end_time;

pthread_t capture_thread_id;
pthread_t ocr_thread_id;
pthread_mutex_t gCapQueueMutex = PTHREAD_MUTEX_INITIALIZER;
queue<CaptureElement> captureQueue;
OcrTaskArgs gOcrArgs;

// fill tensor with mean and scale and trans layout: nhwc -> nchw, neon speed up
void NeonMeanScale(const float *din, float *dout, int size,
                   const std::vector<float> mean,
                   const std::vector<float> scale) {
  if (mean.size() != 3 || scale.size() != 3) {
    std::cerr << "[ERROR] mean or scale size must equal to 3\n";
    exit(1);
  }
  float32x4_t vmean0 = vdupq_n_f32(mean[0]);
  float32x4_t vmean1 = vdupq_n_f32(mean[1]);
  float32x4_t vmean2 = vdupq_n_f32(mean[2]);
  float32x4_t vscale0 = vdupq_n_f32(scale[0]);
  float32x4_t vscale1 = vdupq_n_f32(scale[1]);
  float32x4_t vscale2 = vdupq_n_f32(scale[2]);

  float *dout_c0 = dout;
  float *dout_c1 = dout + size;
  float *dout_c2 = dout + size * 2;

  int i = 0;
  for (; i < size - 3; i += 4) {
    float32x4x3_t vin3 = vld3q_f32(din);
    float32x4_t vsub0 = vsubq_f32(vin3.val[0], vmean0);
    float32x4_t vsub1 = vsubq_f32(vin3.val[1], vmean1);
    float32x4_t vsub2 = vsubq_f32(vin3.val[2], vmean2);
    float32x4_t vs0 = vmulq_f32(vsub0, vscale0);
    float32x4_t vs1 = vmulq_f32(vsub1, vscale1);
    float32x4_t vs2 = vmulq_f32(vsub2, vscale2);
    vst1q_f32(dout_c0, vs0);
    vst1q_f32(dout_c1, vs1);
    vst1q_f32(dout_c2, vs2);

    din += 12;
    dout_c0 += 4;
    dout_c1 += 4;
    dout_c2 += 4;
  }
  for (; i < size; i++) {
    *(dout_c0++) = (*(din++) - mean[0]) * scale[0];
    *(dout_c1++) = (*(din++) - mean[1]) * scale[1];
    *(dout_c2++) = (*(din++) - mean[2]) * scale[2];
  }
}

// resize image to a size multiple of 32 which is required by the network
cv::Mat DetResizeImg(const cv::Mat img, int max_size_len,
                     std::vector<float> &ratio_hw) {
  int w = img.cols;
  int h = img.rows;

  float ratio = 1.f;
  int max_wh = w >= h ? w : h;
  if (max_wh > max_size_len) {
    if (h > w) {
      ratio = static_cast<float>(max_size_len) / static_cast<float>(h);
    } else {
      ratio = static_cast<float>(max_size_len) / static_cast<float>(w);
    }
  }

  int resize_h = static_cast<int>(float(h) * ratio);
  int resize_w = static_cast<int>(float(w) * ratio);
  if (resize_h % 32 == 0)
    resize_h = resize_h;
  else if (resize_h / 32 < 1 + 1e-5)
    resize_h = 32;
  else
    resize_h = (resize_h / 32 - 1) * 32;

  if (resize_w % 32 == 0)
    resize_w = resize_w;
  else if (resize_w / 32 < 1 + 1e-5)
    resize_w = 32;
  else
    resize_w = (resize_w / 32 - 1) * 32;

  cv::Mat resize_img;
  cv::resize(img, resize_img, cv::Size(resize_w, resize_h));

  ratio_hw.push_back(static_cast<float>(resize_h) / static_cast<float>(h));
  ratio_hw.push_back(static_cast<float>(resize_w) / static_cast<float>(w));
  return resize_img;
}

cv::Mat RunClsModel(cv::Mat img, std::shared_ptr<PaddlePredictor> predictor_cls,
                    const float thresh = 0.9) {
  std::vector<float> mean = {0.5f, 0.5f, 0.5f};
  std::vector<float> scale = {1 / 0.5f, 1 / 0.5f, 1 / 0.5f};

  cv::Mat srcimg;
  img.copyTo(srcimg);
  cv::Mat crop_img;
  img.copyTo(crop_img);
  cv::Mat resize_img;

  int index = 0;
  float wh_ratio =
      static_cast<float>(crop_img.cols) / static_cast<float>(crop_img.rows);

  resize_img = ClsResizeImg(crop_img);
  resize_img.convertTo(resize_img, CV_32FC3, 1 / 255.f);

  const float *dimg = reinterpret_cast<const float *>(resize_img.data);

  std::unique_ptr<Tensor> input_tensor0(std::move(predictor_cls->GetInput(0)));
  input_tensor0->Resize({1, 3, resize_img.rows, resize_img.cols});
  auto *data0 = input_tensor0->mutable_data<float>();

  NeonMeanScale(dimg, data0, resize_img.rows * resize_img.cols, mean, scale);
  // Run CLS predictor
  predictor_cls->Run();

  // Get output and run postprocess
  std::unique_ptr<const Tensor> softmax_out(
      std::move(predictor_cls->GetOutput(0)));
  auto *softmax_scores = softmax_out->mutable_data<float>();
  auto softmax_out_shape = softmax_out->shape();
  float score = 0;
  int label = 0;
  for (int i = 0; i < softmax_out_shape[1]; i++) {
    if (softmax_scores[i] > score) {
      score = softmax_scores[i];
      label = i;
    }
  }
  if (label % 2 == 1 && score > thresh) {
    cv::rotate(srcimg, srcimg, 1);
  }
  return srcimg;
}

void RunRecModel(std::vector<std::vector<std::vector<int>>> boxes, cv::Mat img,
                 std::shared_ptr<PaddlePredictor> predictor_crnn,
                 std::vector<std::string> &rec_text,
                 std::vector<float> &rec_text_score,
                 std::vector<std::string> charactor_dict,
                 std::shared_ptr<PaddlePredictor> predictor_cls) {
  std::vector<float> mean = {0.5f, 0.5f, 0.5f};
  std::vector<float> scale = {1 / 0.5f, 1 / 0.5f, 1 / 0.5f};

  cv::Mat srcimg;
  img.copyTo(srcimg);
  cv::Mat crop_img;
  cv::Mat resize_img;

  int index = 0;
  for (int i = boxes.size() - 1; i >= 0; i--) {
    crop_img = GetRotateCropImage(srcimg, boxes[i]);
    // if (use_direction_classify >= 1) {
    //   crop_img = RunClsModel(crop_img, predictor_cls);
    // }
    float wh_ratio =
        static_cast<float>(crop_img.cols) / static_cast<float>(crop_img.rows);

    resize_img = CrnnResizeImg(crop_img, wh_ratio);
    resize_img.convertTo(resize_img, CV_32FC3, 1 / 255.f);

    const float *dimg = reinterpret_cast<const float *>(resize_img.data);

    std::unique_ptr<Tensor> input_tensor0(
        std::move(predictor_crnn->GetInput(0)));
    input_tensor0->Resize({1, 3, resize_img.rows, resize_img.cols});
    auto *data0 = input_tensor0->mutable_data<float>();

    NeonMeanScale(dimg, data0, resize_img.rows * resize_img.cols, mean, scale);
    //// Run CRNN predictor
    predictor_crnn->Run();

    // Get output and run postprocess
    std::unique_ptr<const Tensor> output_tensor0(
        std::move(predictor_crnn->GetOutput(0)));
    auto *predict_batch = output_tensor0->data<float>();
    auto predict_shape = output_tensor0->shape();

    // ctc decode
    std::string str_res;
    int argmax_idx;
    int last_index = 0;
    float score = 0.f;
    int count = 0;
    float max_value = 0.0f;

    for (int n = 0; n < predict_shape[1]; n++) {
      argmax_idx = int(Argmax(&predict_batch[n * predict_shape[2]],
                              &predict_batch[(n + 1) * predict_shape[2]]));
      max_value =
          float(*std::max_element(&predict_batch[n * predict_shape[2]],
                                  &predict_batch[(n + 1) * predict_shape[2]]));
      if (argmax_idx > 0 && (!(n > 0 && argmax_idx == last_index))) {
        score += max_value;
        count += 1;
        str_res += charactor_dict[argmax_idx];
      }
      last_index = argmax_idx;
    }
    score /= count;
    rec_text.push_back(str_res);
    rec_text_score.push_back(score);
  }
}

std::vector<std::vector<std::vector<int>>>
RunDetModel(std::shared_ptr<PaddlePredictor> predictor, cv::Mat img,
            std::map<std::string, double> Config) {
  // Read img
  int max_side_len = int(Config["max_side_len"]);
  int det_db_use_dilate = int(Config["det_db_use_dilate"]);

  cv::Mat srcimg;
  img.copyTo(srcimg);

  std::vector<float> ratio_hw;
  img = DetResizeImg(img, max_side_len, ratio_hw);
  cv::Mat img_fp;
  img.convertTo(img_fp, CV_32FC3, 1.0 / 255.f);

  // Prepare input data from image
  std::unique_ptr<Tensor> input_tensor0(std::move(predictor->GetInput(0)));
  input_tensor0->Resize({1, 3, img_fp.rows, img_fp.cols});
  auto *data0 = input_tensor0->mutable_data<float>();

  std::vector<float> mean = {0.485f, 0.456f, 0.406f};
  std::vector<float> scale = {1 / 0.229f, 1 / 0.224f, 1 / 0.225f};
  const float *dimg = reinterpret_cast<const float *>(img_fp.data);
  NeonMeanScale(dimg, data0, img_fp.rows * img_fp.cols, mean, scale);

  // Run predictor
  predictor->Run();

  // Get output and post process
  std::unique_ptr<const Tensor> output_tensor(
      std::move(predictor->GetOutput(0)));
  auto *outptr = output_tensor->data<float>();
  auto shape_out = output_tensor->shape();

  // Save output
  float pred[shape_out[2] * shape_out[3]];
  unsigned char cbuf[shape_out[2] * shape_out[3]];

  for (int i = 0; i < int(shape_out[2] * shape_out[3]); i++) {
    pred[i] = static_cast<float>(outptr[i]);
    cbuf[i] = static_cast<unsigned char>((outptr[i]) * 255);
  }

  cv::Mat cbuf_map(shape_out[2], shape_out[3], CV_8UC1,
                   reinterpret_cast<unsigned char *>(cbuf));
  cv::Mat pred_map(shape_out[2], shape_out[3], CV_32F,
                   reinterpret_cast<float *>(pred));

  const double threshold = double(Config["det_db_thresh"]) * 255;
  const double max_value = 255;
  cv::Mat bit_map;
  cv::threshold(cbuf_map, bit_map, threshold, max_value, cv::THRESH_BINARY);
  if (det_db_use_dilate == 1) {
    cv::Mat dilation_map;
    cv::Mat dila_ele =
        cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::dilate(bit_map, dilation_map, dila_ele);
    bit_map = dilation_map;
  }
  auto boxes = BoxesFromBitmap(pred_map, bit_map, Config);

  std::vector<std::vector<std::vector<int>>> filter_boxes =
      FilterTagDetRes(boxes, ratio_hw[0], ratio_hw[1], srcimg);

  return filter_boxes;
}

//Detect text by using traditional method
std::vector<std::vector<std::vector<int>>>
RunDetMethod(cv::Mat img, std::map<std::string, double> Config) {
  // Read img
  int max_side_len = int(Config["max_side_len"]);

  cv::Mat imgBuf1,imgBuf2;
  cv::Mat grad_x, grad_y;
  cv::Mat abs_grad_x,abs_grad_y;

  cv::cvtColor(img, imgBuf1, cv::COLOR_BGR2GRAY);

  //计算X方向的梯度
  cv::Sobel(imgBuf1, grad_x, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
  cv::convertScaleAbs(grad_x, abs_grad_x);
  //计算y方向的梯度
  cv::Sobel(imgBuf1, grad_y, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);
  cv::convertScaleAbs(grad_y, abs_grad_y);
  //合并梯度
  cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, imgBuf1);
  //二值化
  //cv::adaptiveThreshold(imgBuf1, imgBuf2, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, -2);
  cv::threshold(imgBuf1, imgBuf2, 25, 255, cv::THRESH_BINARY);

  //腐蚀膨胀
  cv::Mat element1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
  cv::Mat element2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
 
  //膨胀一次，让轮廓突出
  cv::dilate(imgBuf2, imgBuf1, element2);
 
  //腐蚀一次，去掉细节
  cv::erode(imgBuf1, imgBuf2, element1);
 
  //再次膨胀，让轮廓明显一些
  cv::dilate(imgBuf2, imgBuf1, element2);

  auto boxes = BoxesFromBitmap(imgBuf1, Config);
  
  // std::vector<std::vector<std::vector<int>>> filter_boxes =
  //     FilterTagDetRes(boxes, ratio_hw[0], ratio_hw[1], img);

  return boxes;
}

std::shared_ptr<PaddlePredictor> loadModel(std::string model_file) {
  MobileConfig config;
  config.set_model_from_file(model_file);

  std::shared_ptr<PaddlePredictor> predictor =
      CreatePaddlePredictor<MobileConfig>(config);
  return predictor;
}

cv::Mat Visualization(cv::Mat srcimg,
                      std::vector<std::vector<std::vector<int>>> boxes) {
  cv::Point rook_points[boxes.size()][4];
  for (int n = 0; n < boxes.size(); n++) {
    for (int m = 0; m < boxes[0].size(); m++) {
      rook_points[n][m] = cv::Point(static_cast<int>(boxes[n][m][0]),
                                    static_cast<int>(boxes[n][m][1]));
    }
  }
  cv::Mat img_vis;
  srcimg.copyTo(img_vis);
  for (int n = 0; n < boxes.size(); n++) {
    const cv::Point *ppt[1] = {rook_points[n]};
    int npt[] = {4};
    cv::polylines(img_vis, ppt, npt, 1, 1, CV_RGB(0, 255, 0), 2, 8, 0);
  }

  cv::imwrite("./vis.jpg", img_vis);
  std::cout << "The detection visualized image saved in ./vis.jpg" << std::endl;
  return img_vis;
}

std::vector<std::string> split(const std::string &str,
                               const std::string &delim) {
  std::vector<std::string> res;
  if ("" == str)
    return res;
  char *strs = new char[str.length() + 1];
  std::strcpy(strs, str.c_str());

  char *d = new char[delim.length() + 1];
  std::strcpy(d, delim.c_str());

  char *p = std::strtok(strs, d);
  while (p) {
    string s = p;
    res.push_back(s);
    p = std::strtok(NULL, d);
  }

  return res;
}

std::map<std::string, double> LoadConfigTxt(std::string config_path) {
  auto config = ReadDict(config_path);

  std::map<std::string, double> dict;
  for (int i = 0; i < config.size(); i++) {
    std::vector<std::string> res = split(config[i], " ");
    dict[res[0]] = stod(res[1]);
  }
  return dict;
}

static void showDebugTimeInfo(void){
  auto duration_init =
      std::chrono::duration_cast<std::chrono::microseconds>(end_init - start_init_time);

  auto duration_LoadImg =
      std::chrono::duration_cast<std::chrono::microseconds>(end_load_img - start_ocr_time);

  auto duration_RunDetModel =
      std::chrono::duration_cast<std::chrono::microseconds>(end_RunDetModel - end_load_img);

  auto duration_RunRecModel =
      std::chrono::duration_cast<std::chrono::microseconds>(end_time - end_RunDetModel);

  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_ocr_time);

  // std::cout << "初始化花费了"
  //           << double(duration_init.count()) *
  //                  std::chrono::microseconds::period::num /
  //                  std::chrono::microseconds::period::den
  //           << "秒" << std::endl;
  std::cout << "加载图片花费了"
            << double(duration_LoadImg.count()) *
                   std::chrono::microseconds::period::num /
                   std::chrono::microseconds::period::den
            << "秒" << std::endl;
  std::cout << "运行检测模型花费了"
            << double(duration_RunDetModel.count()) *
                   std::chrono::microseconds::period::num /
                   std::chrono::microseconds::period::den
            << "秒" << std::endl;
  std::cout << "运行识别模型花费了"
            << double(duration_RunRecModel.count()) *
                   std::chrono::microseconds::period::num /
                   std::chrono::microseconds::period::den
            << "秒" << std::endl;
  std::cout << "识别阶段总共花费了"
            << double(duration.count()) *
                   std::chrono::microseconds::period::num /
                   std::chrono::microseconds::period::den
            << "秒" << std::endl;
}

void initOcrArgs(std::string det_model_file, std::string rec_model_file,
                std::string cls_model_file, std::string dict_path){
    gOcrArgs.det_model_file = det_model_file;
    gOcrArgs.rec_model_file = rec_model_file;
    gOcrArgs.cls_model_file = cls_model_file;
    gOcrArgs.dict_path = dict_path;
}

void* ocrTask(void* arg)
{
    char savePath[MAX_CAPTURE_FILE_NAME_LENGTH] = "";
    start_init_time = std::chrono::system_clock::now();//[debug]
    //// load config from txt file
    auto Config = LoadConfigTxt("./config.txt");
    // int use_direction_classify = int(Config["use_direction_classify"]);
    auto det_predictor = loadModel(gOcrArgs.det_model_file);
    auto rec_predictor = loadModel(gOcrArgs.rec_model_file);
    auto cls_predictor = loadModel(gOcrArgs.cls_model_file);
    auto charactor_dict = ReadDict(gOcrArgs.dict_path);
    charactor_dict.insert(charactor_dict.begin(), "#"); // blank char for ctc
    charactor_dict.push_back(" ");

    end_init = std::chrono::system_clock::now();//[debug]
    while(1)
    {   
        //for cancel
        pthread_testcancel();
        //check queue start
        pthread_mutex_lock(&gCapQueueMutex);//[LOCK]
        if (captureQueue.empty()){
          pthread_mutex_unlock(&gCapQueueMutex);//[UNLOCK]
          continue;
        }

        CaptureElement element = captureQueue.front();
        memcpy(savePath, element.savePath, sizeof(savePath));
        captureQueue.pop();
        pthread_mutex_unlock(&gCapQueueMutex);//[UNLOCK]
        //check queue end
        start_ocr_time = std::chrono::system_clock::now();//[debug]

        std::string img_path = savePath;
        std::cout << "[DEBUG] ocr check img_path: " << img_path <<std::endl;
        cv::Mat srcimg = cv::imread(img_path, cv::IMREAD_COLOR);
        //resize for acceleration
        cv::Mat resize_srcimg;
        cv::resize(srcimg, resize_srcimg, cv::Size(80, 60), 0.f, 0.f,
             cv::INTER_LINEAR);

        end_load_img = std::chrono::system_clock::now();//[debug]

        auto boxes = RunDetModel(det_predictor, resize_srcimg, Config);

        std::vector<std::string> rec_text;
        std::vector<float> rec_text_score;

        end_RunDetModel = std::chrono::system_clock::now();//[debug]

        RunRecModel(boxes, resize_srcimg, rec_predictor, rec_text, rec_text_score,
                    charactor_dict, cls_predictor);

        end_time = std::chrono::system_clock::now();//[debug]
        // debug info
        auto img_vis = Visualization(resize_srcimg, boxes);
        for (int i = 0; i < rec_text.size(); i++) {
          std::cout << i << "\t" << rec_text[i] << "\t" << rec_text_score[i]
                    << std::endl;
        }
        showDebugTimeInfo();
    }
    return NULL;
}

void* captureTask(void* arg)
{
    char savePath[MAX_CAPTURE_FILE_NAME_LENGTH] = "";
    while(1)
    {
        //for cancel
        pthread_testcancel();
        //check queue start
        pthread_mutex_lock(&gCapQueueMutex);//[LOCK]
        if (captureQueue.size() >= MAX_CAPTURE_SIZE){
          pthread_mutex_unlock(&gCapQueueMutex);//[UNLOCK]
          continue;
        }
        
        unsigned int new_index = 0;
        if (!captureQueue.empty()){
          memset(savePath, 0, sizeof(savePath));
          auto last_element = captureQueue.back();
          new_index = last_element.index + 1;
          if (new_index >= MAX_CAPTURE_SIZE)//假的循环队列
          {
              new_index = 0;
          }
        }
        sprintf(savePath, "test_%d.jpg", new_index);
        auto element = CaptureElement(savePath, new_index);
        captureQueue.push(element);
        //check queue end
        //save picture
        v4l2_capture_one_frame(savePath);
        pthread_mutex_unlock(&gCapQueueMutex);//[UNLOCK]
    }
    return NULL;
}

void startCaptureTask(void){
    if(pthread_create(&capture_thread_id, NULL, captureTask, NULL))
    {
        std::cout << "[ERROR] CaptureTask CAN not start" << std::endl;
        return;
    }
}

void startOcrTask(void){
    if(pthread_create(&ocr_thread_id, NULL, ocrTask, NULL))
    {
        std::cout << "[ERROR] OcrTask CAN not start" << std::endl;
        return;
    }
}

void stopTask(void)
{
    pthread_cancel(capture_thread_id);
    pthread_cancel(ocr_thread_id);
}

void loopTask(void)
{
    pthread_join(capture_thread_id, NULL);
    pthread_join(ocr_thread_id, NULL);
}