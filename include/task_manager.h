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
#pragma once

#include <chrono>
#include <arm_neon.h>
#include <signal.h>
#include <pthread.h>
#include <queue>
#include "cls_process.h"
#include "crnn_process.h"
#include "db_post_process.h"
#include "v4l2_camera.h"
#include "paddle_api.h" // NOLINT

#define MAX_CAPTURE_FILE_NAME_LENGTH   20
#define MAX_CONFIG_FILE_NAME_LENGTH    50

class OcrTaskArgs {
  public:
    std::string det_model_file;
    std::string rec_model_file;
    std::string cls_model_file;
    std::string dict_path;
};

class CaptureElement {
  public:
    char savePath[MAX_CAPTURE_FILE_NAME_LENGTH];   //字符串描述信息
    unsigned int index;  //像素格式
  CaptureElement(const char* filename, unsigned int id){
  	memcpy(savePath, filename, sizeof(savePath));
  	index = id;
  }
};


void initOcrArgs(std::string det_model_file, std::string rec_model_file,
				 std::string cls_model_file, std::string dict_path);
void startOcrTask(void);
void startCaptureTask(void);
void loopTask(void);
void stopTask(void);