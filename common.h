#ifndef COMMON_H
#define COMMON_H

//环境变量
#define DEBUG  //调试模式，显示调试信息
#define APPLE  //APPLE MACOS
#define MAX_BUFFER_SIZE 256  //默认缓冲区大小

//相机位置
#define CAMERA_POSITION_X_INIT 0.0f
#define CAMERA_POSITION_Y_INIT 0.0f
#define CAMERA_POSITION_Z_INIT 1.0f

//相机朝向
#define CAMERA_DIRECTION_X_INIT 0.0f
#define CAMERA_DIRECTION_Y_INIT 0.0f
#define CAMERA_DIRECTION_Z_INIT -1.0f
#define CAMERA_UP_X_INIT 0.0f
#define CAMERA_UP_Y_INIT 1.0f
#define CAMERA_UP_Z_INIT 0.0f

//缩放比例， 滚轮每次向上滚动图像放大为原先的1/0.88倍， 向下滚动缩小为原先的0.88倍
#define CAMERA_SCALE_RATE 0.88f

//Volume采样率，初始采样率为0.5f, 最大采样率为10.0f
#define VOLUME_SAMPLINGRATE_INIT 0.5f
#define RENDER_SAMPLING_RATE_MAX 10.0f

//初始截断距离与最大截断距离，这个根据镜头与物体的距离而定
#define RENDER_CLIPPING_INIT 0.5f
#define RENDER_NEAR_CLIPPING_MAX 2.0f

//镜面反射率， 默认0.3f
#define RENDER_SPECULAR_RATE_INIT 0.3f
#define RENDER_SPECULAR_RATE_MAX 1.0f

//RenderWidget的大小，同时也是图像的大小
#define RENDER_WIDGET_WIDTH 768
#define RENDER_WIDGET_HEIGHT 768

//背景光初始强度与最大值
#define LIGHT_AMBIENT_INTENSITY_INIT 1.0f
#define LIGHT_AMBIENT_INTENSITY_MAX 100.0f

//普通光线的初始值与最大值
#define LIGHT_DISTANT_INTENSITY_INIT 1.0f
#define LIGHT_DISTANT_INTENSITY_MAX 100.0f

//枚举：光的颜色
enum {
    SPECULAR_COLOR_RED, SPECULAR_COLOR_GREEN, SPECULAR_COLOR_BLUE
};

#endif //COMMON_H
