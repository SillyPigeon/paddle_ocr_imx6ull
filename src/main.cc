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

//进程退出处理，程序调用 exit 或 main函数return 或 最后一个线程正常退出
void signalHandle(int sig)
{
    //handle
    std::cerr << "[WARNING] progress exit\n";
    v4l2_off();
}

void exitHandle(void)
{
    //handle
    std::cerr << "[WARNING] progress exit\n";
    v4l2_off();
}

int main(int argc, char **argv) {
  if (argc < 4) {
    std::cerr << "[ERROR] usage: " << argv[0]
              << " det_model_file cls_model_file rec_model_file camera_device \n";
    exit(EXIT_FAILURE);
  }

  //设置参数
  std::string det_model_file = argv[1];
  std::string rec_model_file = argv[2];
  std::string cls_model_file = argv[3];
  std::string dict_path = "./ppocr_keys_v1.txt";
  initOcrArgs(det_model_file, rec_model_file, cls_model_file, dict_path);
  char* camera_device = argv[4];
  
  //初始化设备
  if (init_camera(camera_device))
  {
    std::cerr << "[ERROR] init_camera\n";
    exit(EXIT_FAILURE);
  }

  signal(SIGINT, signalHandle);
  signal(SIGTSTP, signalHandle);
  atexit(exitHandle);

  //开启循环捕获图片任务
  startCaptureTask();

  //开启OcrTask
  startOcrTask();

  //循环检测
  loopTask();
  
  //exit code
  exit(EXIT_SUCCESS);
}