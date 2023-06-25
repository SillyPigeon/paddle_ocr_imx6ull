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

#include "v4l2_camera.h"

#define FB_DEV              "/dev/fb0"      //LCD设备节点
#define FRAMEBUFFER_COUNT   1               //帧缓冲数量
#define RGB_PIXEL_BYTES     3               //RGB像素占用位宽

static int width;                       //LCD宽度
static int height;                      //LCD高度
static unsigned short *screen_base = NULL;//LCD显存基地址
static int fb_fd = -1;                  //LCD设备文件描述符
static int v4l2_fd = -1;                //摄像头设备文件描述符
static cam_buf_info buf_infos[FRAMEBUFFER_COUNT];
static cam_fmt cam_fmts[10];
static int frm_width, frm_height;   //视频帧宽度和高度

static void yuyv_to_rgb(unsigned char* yuv,unsigned char* rgb);
static void rgb_to_bmp(unsigned char* pdata, FILE* bmp_fd);

int fb_dev_init(void)
{
    struct fb_var_screeninfo fb_var = {0};
    struct fb_fix_screeninfo fb_fix = {0};
    unsigned long screen_size;

    /* 打开framebuffer设备 */
    fb_fd = open(FB_DEV, O_RDWR);
    if (0 > fb_fd) {
        fprintf(stderr, "open error: %s: %s\n", FB_DEV, strerror(errno));
        return -1;
    }

    /* 获取framebuffer设备信息 */
    ioctl(fb_fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fb_fd, FBIOGET_FSCREENINFO, &fb_fix);

    screen_size = fb_fix.line_length * fb_var.yres;
    width = fb_var.xres;
    height = fb_var.yres;

    /* 内存映射 */
    screen_base = mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if (MAP_FAILED == (void *)screen_base) {
        perror("mmap error");
        close(fb_fd);
        return -1;
    }

    /* LCD背景刷白 */
    memset(screen_base, 0xFF, screen_size);
    return 0;
}

int v4l2_dev_init(const char *device)
{
    struct v4l2_capability cap = {0};

    /* 打开摄像头 */
    v4l2_fd = open(device, O_RDWR);
    if (0 > v4l2_fd) {
        fprintf(stderr, "open error: %s: %s\n", device, strerror(errno));
        return -1;
    }

    /* 查询设备功能 */
    ioctl(v4l2_fd, VIDIOC_QUERYCAP, &cap);

    /* 判断是否是视频采集设备 */
    if (!(V4L2_CAP_VIDEO_CAPTURE & cap.capabilities)) {
        fprintf(stderr, "Error: %s: No capture video device!\n", device);
        close(v4l2_fd);
        return -1;
    }

    return 0;
}

void v4l2_enum_formats(void)
{
    struct v4l2_fmtdesc fmtdesc = {0};

    /* 枚举摄像头所支持的所有像素格式以及描述信息 */
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while (0 == ioctl(v4l2_fd, VIDIOC_ENUM_FMT, &fmtdesc)) {

        // 将枚举出来的格式以及描述信息存放在数组中
        cam_fmts[fmtdesc.index].pixelformat = fmtdesc.pixelformat;
        strcpy(cam_fmts[fmtdesc.index].description, fmtdesc.description);
        fmtdesc.index++;
    }
}

void v4l2_print_formats(void)
{
    struct v4l2_frmsizeenum frmsize = {0};
    struct v4l2_frmivalenum frmival = {0};
    int i;

    frmsize.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    frmival.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    for (i = 0; cam_fmts[i].pixelformat; i++) {

        printf("format<0x%x>, description<%s>\n", cam_fmts[i].pixelformat,
                    cam_fmts[i].description);

        /* 枚举出摄像头所支持的所有视频采集分辨率 */
        frmsize.index = 0;
        frmsize.pixel_format = cam_fmts[i].pixelformat;
        frmival.pixel_format = cam_fmts[i].pixelformat;
        while (0 == ioctl(v4l2_fd, VIDIOC_ENUM_FRAMESIZES, &frmsize)) {

            printf("size<%d*%d> ",
                    frmsize.discrete.width,
                    frmsize.discrete.height);
            frmsize.index++;

            /* 获取摄像头视频采集帧率 */
            frmival.index = 0;
            frmival.width = frmsize.discrete.width;
            frmival.height = frmsize.discrete.height;
            while (0 == ioctl(v4l2_fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmival)) {

                printf("<%dfps>", frmival.discrete.denominator /
                        frmival.discrete.numerator);
                frmival.index++;
            }
            printf("\n");
        }
        printf("\n");
    }
}

int v4l2_set_format(int setWidth, int setHeight, int setFps, int setPixelType)
{
    struct v4l2_format fmt = {0};
    struct v4l2_streamparm streamparm = {0};

    /* 设置帧格式 */
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;//type类型
    fmt.fmt.pix.width = setWidth;  //视频帧宽度
    fmt.fmt.pix.height = setHeight;//视频帧高度
    fmt.fmt.pix.pixelformat = setPixelType;  //像素格式
    if (0 > ioctl(v4l2_fd, VIDIOC_S_FMT, &fmt)) {
        fprintf(stderr, "ioctl error: VIDIOC_S_FMT: %s\n", strerror(errno));
        return -1;
    }

    frm_width = fmt.fmt.pix.width;  //获取实际的帧宽度
    frm_height = fmt.fmt.pix.height;//获取实际的帧高度
    printf("设置捕获设备视频帧大小<%d * %d>\n", frm_width, frm_height);

    /* 获取streamparm */
    streamparm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(v4l2_fd, VIDIOC_G_PARM, &streamparm);

    /** 判断是否支持帧率设置 **/
    if (V4L2_CAP_TIMEPERFRAME & streamparm.parm.capture.capability) {
        streamparm.parm.capture.timeperframe.numerator = 1;
        streamparm.parm.capture.timeperframe.denominator = setFps;//30fps
        if (0 > ioctl(v4l2_fd, VIDIOC_S_PARM, &streamparm)) {
            fprintf(stderr, "ioctl error: VIDIOC_S_PARM: %s\n", strerror(errno));
            return -1;
        }
    }

    return 0;
}

int v4l2_set_format_for_yuyv(int setWidth, int setHeight, int setFps)
{
    return v4l2_set_format(setWidth, 
                           setHeight, 
                           setFps,
                           V4L2_PIX_FMT_YUYV);
}

int v4l2_init_buffer(void)
{
    struct v4l2_requestbuffers reqbuf = {0};
    struct v4l2_buffer buf = {0};

    /* 申请帧缓冲 */
    reqbuf.count = FRAMEBUFFER_COUNT;       //帧缓冲的数量
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    if (0 > ioctl(v4l2_fd, VIDIOC_REQBUFS, &reqbuf)) {
        fprintf(stderr, "ioctl error: VIDIOC_REQBUFS: %s\n", strerror(errno));
        return -1;
    }

    /* 建立内存映射 */
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    for (buf.index = 0; buf.index < FRAMEBUFFER_COUNT; buf.index++) {

        ioctl(v4l2_fd, VIDIOC_QUERYBUF, &buf);
        buf_infos[buf.index].length = buf.length;
        buf_infos[buf.index].start = mmap(NULL, buf.length,
                PROT_READ | PROT_WRITE, MAP_SHARED,
                v4l2_fd, buf.m.offset);
        if (MAP_FAILED == buf_infos[buf.index].start) {
            perror("mmap error");
            return -1;
        }
    }

    /* 入队 */
    for (buf.index = 0; buf.index < FRAMEBUFFER_COUNT; buf.index++) {

        if (0 > ioctl(v4l2_fd, VIDIOC_QBUF, &buf)) {
            fprintf(stderr, "ioctl error: VIDIOC_QBUF: %s\n", strerror(errno));
            return -1;
        }
    }

    return 0;
}

int v4l2_stream_on(void)
{
    /* 打开摄像头、摄像头开始采集数据 */
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (0 > ioctl(v4l2_fd, VIDIOC_STREAMON, &type)) {
        fprintf(stderr, "ioctl error: VIDIOC_STREAMON: %s\n", strerror(errno));
        return -1;
    }

    printf("摄像头开始采集数据。\n");

    return 0;
}

void v4l2_capture_one_frame(const char* savePath)
{
    struct v4l2_buffer readbuffer = {0};
    unsigned short *base;
    unsigned short *start;
    int min_w, min_h;
    int  file_index = 0;

    if (width > frm_width)
        min_w = frm_width;
    else
        min_w = width;
    if (height > frm_height)
        min_h = frm_height;
    else
        min_h = height;

    readbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    readbuffer.memory = V4L2_MEMORY_MMAP;
    //for (int i = 0; i < 2; ++i)
    {
        for (readbuffer.index = 0; readbuffer.index < FRAMEBUFFER_COUNT; readbuffer.index++, file_index++)
        {
            //出队列
            ioctl(v4l2_fd, VIDIOC_DQBUF, &readbuffer);
            //存储图片
            //sprintf(filename, "test.jpg");
            //sprintf(filename, "test_%d.jpg", file_index);
            FILE *file=fopen(savePath, "w");  //打开一个文件
            fwrite( buf_infos[readbuffer.index].start, readbuffer.length, 1, file);//写入文件
            fclose(file);    //写入完成，关闭文件
            // 数据处理完之后、再入队、往复, 从内核取出后需要放回(释放)
            ioctl(v4l2_fd, VIDIOC_QBUF, &readbuffer);
        }
    }
}

void v4l2_capture_one_frame_for_yuyv(const char* savePath)
{
    struct v4l2_buffer readbuffer = {0};
    unsigned char rgb_buffers[(CAMERA_FORMAT_WIDTH * CAMERA_FORMAT_HEIGHT) * RGB_PIXEL_BYTES] = {};
    unsigned short *base;
    unsigned short *start;
    int min_w, min_h;
    int  file_index = 0;

    if (width > frm_width)
        min_w = frm_width;
    else
        min_w = width;
    if (height > frm_height)
        min_h = frm_height;
    else
        min_h = height;

    readbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    readbuffer.memory = V4L2_MEMORY_MMAP;
    //for (int i = 0; i < 2; ++i)
    {
        for (readbuffer.index = 0; readbuffer.index < FRAMEBUFFER_COUNT; readbuffer.index++, file_index++)
        {
            //出队列
            ioctl(v4l2_fd, VIDIOC_DQBUF, &readbuffer);

            // for DEBUG to save yuv
            // FILE *file_yuv=fopen("yuv_test.yuv", "w");  //打开一个文件
            // fwrite( buf_infos[readbuffer.index].start, readbuffer.length, 1, file_yuv);//写入文件
            // fclose(file_yuv); //写入完成，关闭文件

            yuyv_to_rgb((unsigned char*)buf_infos[readbuffer.index].start, rgb_buffers);

            //存储图片
            FILE *file=fopen(savePath, "w");  //打开一个文件
            rgb_to_bmp(rgb_buffers, file);
            fclose(file);    //写入完成，关闭文件
            // 数据处理完之后、再入队、往复, 从内核取出后需要放回(释放)
            ioctl(v4l2_fd, VIDIOC_QBUF, &readbuffer);
        }
    }
}

static void yuyv_to_rgb(unsigned char* yuv, unsigned char* rgb)
{
    //Init first two pixels
    unsigned int i;
    unsigned char *y0, *u0, *y1, *v0;
    unsigned char *r0, *g0, *b0, *r1, *g1, *b1;
   
    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;

    if((yuv == NULL) || (rgb == NULL)){
        return;
    }

    //printf("[DEBUG]: [%d][%d],[%d][%d]\n",*y0,*u0,*y1,*v0);    
    //transfer by each two pixel
    for(i = (CAMERA_FORMAT_WIDTH * CAMERA_FORMAT_HEIGHT) / 2 ; 
        i > 0; 
        i--)
    {
        y0 = yuv + 0;
        u0 = yuv + 1;
        y1 = yuv + 2;
        v0 = yuv + 3;
  
        r0 = rgb + 0;
        g0 = rgb + 1;
        b0 = rgb + 2;
        r1 = rgb + 3;
        g1 = rgb + 4;
        b1 = rgb + 5;

        bt0 = *y0 + 1.4075 * (*v0 - 128);
        gt0 = *y0 - 0.3455 * (*u0 - 128) - 0.7169 * (*v0 - 128);
        rt0 = *y0 + 1.7790 * (*u0 - 128); 

        bt1 = *y1 + 1.4075 * (*v0 - 128);
        gt1 = *y1 - 0.3455 * (*u0 - 128) - 0.7169 * (*v0 - 128);
        rt1 = *y1 + 1.7790 * (*u0 - 128);
      
        if(rt0 > 250)     rt0 = 255;
        if(rt0< 0)        rt0 = 0;
 
        if(gt0 > 250)     gt0 = 255;
        if(gt0 < 0)       gt0 = 0;
 
        if(bt0 > 250)     bt0 = 255;
        if(bt0 < 0)       bt0 = 0;
 
        if(rt1 > 250)     rt1 = 255;
        if(rt1 < 0)       rt1 = 0;
 
        if(gt1 > 250)     gt1 = 255;
        if(gt1 < 0)       gt1 = 0;  
 
        if(bt1 > 250)     bt1 = 255;
        if(bt1 < 0)       bt1 = 0;
                    
        *r0 = (unsigned char)rt0;
        *g0 = (unsigned char)gt0;
        *b0 = (unsigned char)bt0;
    
        *r1 = (unsigned char)rt1;
        *g1 = (unsigned char)gt1;
        *b1 = (unsigned char)bt1;

        //if not last, switch to next
        if (1 != i){
            yuv += 4;
            rgb += 6;
        }
    }   
}

static void rgb_to_bmp(unsigned char* pdata, FILE* bmp_fd)     
{
    //分别为rgb数据，要保存的bmp文件名 
    int size = CAMERA_FORMAT_WIDTH * CAMERA_FORMAT_HEIGHT * 3 * sizeof(char); // 每个像素点3个字节  
    // 位图第一部分，文件信息  
    BMPFILEHEADER_T bfh; 

    bfh.bfType = (unsigned short) 0x4d42;  //bmp HEAD  
    bfh.bfSize = size + sizeof(BMPFILEHEADER_T) + sizeof(BMPINFOHEADER_T);
    bfh.bfReserved1 = 0; // reserved  
    bfh.bfReserved2 = 0; // reserved  
    bfh.bfOffBits = sizeof(BMPFILEHEADER_T) + sizeof(BMPINFOHEADER_T);//真正的数据的位置 
    // 位图第二部分，数据信息  
    BMPINFOHEADER_T bih;  
    bih.biSize = sizeof(BMPINFOHEADER_T);  
    bih.biWidth = CAMERA_FORMAT_WIDTH;  
    bih.biHeight = -CAMERA_FORMAT_HEIGHT;//BMP图片从最后一个点开始扫描，显示时图片是倒着的，所以用-height，这样图片就正了  
    bih.biPlanes = 1;//为1，不用改  
    bih.biBitCount = 24;  
    bih.biCompression = 0;//不压缩  
    bih.biSizeImage = size;  
 
    bih.biXPelsPerMeter = 0;//像素每米  
  
    bih.biYPelsPerMeter = 0;  
    bih.biClrUsed = 0;//已用过的颜色，为0,与bitcount相同  
    bih.biClrImportant = 0;//每个像素都重要   
    
    fwrite(&bfh, sizeof(BMPFILEHEADER_T), 1, bmp_fd);  
    fwrite(&bih, sizeof(BMPINFOHEADER_T), 1, bmp_fd);  
    fwrite(pdata, size, 1, bmp_fd);   
} 


//***********************************流程函数************************************************//

void v4l2_off(void)
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //停止采集
    ioctl(v4l2_fd,VIDIOC_STREAMOFF,&type);
    for (int i=0; i < FRAMEBUFFER_COUNT; i++) {
        munmap(buf_infos[i].start, buf_infos[i].length);
    }
    close(v4l2_fd);
}

int init_camera(const char* device)
{
  /* 初始化摄像头 */
  if (v4l2_dev_init(device)){
      return -1;
  }

  /* 设置格式 */
  if (v4l2_set_format(CAMERA_FORMAT_WIDTH, 
                      CAMERA_FORMAT_HEIGHT, 
                      CAMERA_FORMAT_FPS,
                      CAMERA_FORMAT_PIXEL_TYPE)){
      return -1;
  }

  /* 初始化帧缓冲：申请、内存映射、入队 */
  if (v4l2_init_buffer()){
      return -1;
  }

  /* 开启视频采集 */
  if (v4l2_stream_on()){
      return -1;
  }

  return 0;
}

// 摄像头单独测试代码
// int main(int argc, char *argv[])
// {
//     if (2 != argc) {
//         fprintf(stderr, "Usage: %s <video_dev>\n", argv[0]);
//         exit(EXIT_FAILURE);
//     }
//     /* 初始化摄像头 */
//     if (v4l2_dev_init(argv[1])){
//         return -1;
//     }

//     /* 设置格式 */
//     if (v4l2_set_format_for_yuyv(CAMERA_FORMAT_WIDTH, 
//                                  CAMERA_FORMAT_HEIGHT, 
//                                  CAMERA_FORMAT_FPS)){
//         return -1;
//     }

//     /* 初始化帧缓冲：申请、内存映射、入队 */
//     if (v4l2_init_buffer()){
//         return -1;
//     }

//     /* 开启视频采集 */
//     if (v4l2_stream_on()){
//        return -1;
//     }

//     /* 枚举所有格式并打印摄像头支持的分辨率及帧率 */
//     // v4l2_enum_formats();
//     // v4l2_print_formats();

//     /* 截取图片, 舍弃前几帧坏信息 */
//     for (int i = 0; i < 5; ++i)
//     {
//         v4l2_capture_one_frame_for_yuyv("test.bmp");
//     }
    
//     //退出程序
//     v4l2_off();
//     exit(EXIT_SUCCESS);
// }
