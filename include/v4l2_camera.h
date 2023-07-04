/***************************************************************
 Copyright © ALIENTEK Co., Ltd. 1998-2021. All rights reserved.
 文件名 : v4l2_camera.c
 作者 : SillyPigeon edit from 邓涛
 版本 : V1.1
 描述 : V4L2摄像头应用编程实战
 其他 : 无
 论坛 : www.openedv.com
 日志 : SillyPigeon edit from 初版 V1.0 2021/7/09 邓涛创建
 ***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <linux/fb.h>
#ifdef __cplusplus
extern "C"{
#endif

#define CAMERA_FORMAT_WIDTH           320
#define CAMERA_FORMAT_HEIGHT          240
#define CAMERA_FORMAT_FPS             30
#define CAMERA_FORMAT_PIXEL_TYPE      V4L2_PIX_FMT_MJPEG

/*** 摄像头像素格式及其描述信息 ***/
typedef struct camera_format {
    unsigned char description[32];  //字符串描述信息
    unsigned int pixelformat;       //像素格式
} cam_fmt;

/*** 描述一个帧缓冲的信息 ***/
typedef struct cam_buf_info {
    unsigned short *start;      //帧缓冲起始地址
    unsigned long length;       //帧缓冲长度
} cam_buf_info;

#pragma pack(push,2)
typedef struct {
    unsigned short    bfType;//0x4d42
    int      bfSize;//sizeof(BMPFILEHEADER_T)+sizeof(BMPINFOHEADER_T)+width*height*3, 整个文件的大小
    unsigned short    bfReserved1;
    unsigned short    bfReserved2;
    int   bfOffBits;//sizeof(BMPFILEHEADER_T)+sizeof(BMPINFOHEADER_T),就是RGB数据的偏移量, 偏移这两个数据结构
} BMPFILEHEADER_T;

typedef struct{
    int      biSize;//sizeof(BMPINFOHEADER_T)
    int      biWidth;
    int      biHeight;
    unsigned short       biPlanes;
    unsigned short       biBitCount;//RGB24, 就是24位
    int      biCompression;//0， 不压缩
    int      biSizeImage;//RGB24数据大小
    int      biXPelsPerMeter;//
    int      biYPelsPerMeter;//
    int      biClrUsed;//0
    int      biClrImportant;//0
} BMPINFOHEADER_T;
#pragma pack(pop)

int fb_dev_init(void);

int v4l2_dev_init(const char *device);

void v4l2_enum_formats(void);

void v4l2_print_formats(void);

int v4l2_set_format(int setWidth, int setHeight, int setFps, int setPixelType);

int v4l2_set_format_for_yuyv(int setWidth, int setHeight, int setFps);

int v4l2_init_buffer(void);

int v4l2_stream_on(void);

void v4l2_off(void);

void v4l2_capture_one_frame(const char* savePath);

void v4l2_capture_one_frame_for_yuyv(const char* savePath);

int init_camera(const char* device);
#ifdef __cplusplus
}
#endif