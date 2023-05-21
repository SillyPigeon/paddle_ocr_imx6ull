# paddle_ocr_imx6ull(版本v0.2)
## 1、简介

此项目是参考文档: https://blog.csdn.net/qq_39056987/article/details/121078743
制作目标应用于工业摄像头的项目

**注意：项目收集于互联网，若用于商用，请联系代码原作者。**

## 2、文件目录说明

#### 2.1 根目录

编译脚本文件 -- build.sh

CMake编译配置文件 -- CMakeLists.txt

#### 2.2  OCR工程源码文件夹 --src --include

存放OCR识别相关操作源文件和头文件 -- *.cpp  *.hpp  *.cc *.h

#### 2.3  OCR依赖库目录 --ocr_lib/

Paddle Lite 环境运行库头文件(api 头文件) -- /include

Paddle OCR 运行依赖的paddle相关动态库  -- /lib

Paddle OCR 运行依赖的opencv相关动态库  -- /opencv-linux-armv7hf

#### 2.4 摄像头相关代码目录 --camera

内含v4l2标准的代码实现，获取调用通用linux摄像头

#### 2.5  训练模型相关的文件目录 --model /

Paddle Lite 模型 -- *.nb

paddle lite 运行配置文件 -- config.txt

paddle lite 运行汉字字典文件 --ppocr_keys_v1.txt

程序运行脚本 -- run.sh

测试图片 -- lite_demo.png

## 3、工程编译说明

本工程在ubuntu 16.04.9、CMake 3.10.3、arm-linux-gnueabihf Linaro 5.4.0 版本

下编译目标架构为armv7hf的可执行文件通过。生成bin的目录于./install/paddle_lite_ocr

备注：也可用官方编译paddle-lite的docker镜像进行此工程编译，挂靠目录工程编译可执行如下指令，进入docker后直接执行./build.sh即可:

docker run -it --name paddleocr_docker -v $PWD/paddle_ocr_imx6ull:/Paddle-ocr --net=host paddlepaddle/paddle-lite /bin/bash

#### 步骤1:

确保arm交叉编译工具和cmake已安装, 可参考:

https://www.paddlepaddle.org.cn/lite/develop/source_compile/linux_x86_compile_arm_linux.html

#### 步骤2：

运行脚本 ./build.sh 

#### 注意事项: 

**1.第一运行编译会不通过，可无视再运行一次脚本build.sh，此问题和环境变量有关，暂未解决**

**2.编译、链接Paddle-Lite的so需要arm-linux-gcc-v5.1(满足C++ 11特性)、CMake 3.10以上的版本**

**3.完成后会自动生成 ./build/ 保存CMake过程文件 和 ./install/ 保存arm-linux可执行文件**

**4.有可能本机安装的arm交叉编译工具无法成功链接工程中的已编译so, 可用paddle源码自行编译**

## 4、运行测试说明

本工程在正点原子 imx6ull 的阿尔法板上运行测试正常，理论上均适用于在armv7hf架构CPU上的linux系统

#### 步骤1：

确保板卡/usr/lib目录中有如下运行所需的动态so库，若无，则需往板卡上推入/编入对应架构已编译的so: 

openMP并行算法库 -- libgomp.so*

paddle-lite 运行环境库 -- libpaddle_light_api_shared.so

opencv 运行环境库 -- libopencv_*.so

(备注: opencv 库在链接的时候，在不同的板卡系统可能会被指定路径不同，要按照具体位置推送so到对应的路径中去)

#### 步骤2：

将对应 可执行文件/配置/模型/字典/测试图片/运行脚本 推入板卡存储空间中:

可执行文件 -- ./install/paddle_lite_ocr/paddle_lite_ocr

神经网络模型 -- ./model/*.nb

运行配置文件 -- ./model/config.txt

运行汉字字典 -- ./model/ppocr_keys_v1.txt

测试图片 -- ./model/lite_demo.png （可自行替换）

测试脚本 run.sh

#### 步骤3：

将步骤2 中的文件置于同一文件夹下，执行测试脚本run.sh。

也可自行按照测试脚本或者代码ocr_db_crnn.cc来运行测试:

./paddle_lite_ocr ch_ppocr_mobile_v2.0_det_slim_opt.nb ch_ppocr_mobile_v2.0_rec_slim_opt.nb ch_ppocr_mobile_v2.0_cls_slim_opt.nb lite_demo.png ppocr_keys_v1.txt
