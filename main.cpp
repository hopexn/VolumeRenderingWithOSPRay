//C++
#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QImage>
#include <QPixmap>

//C
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "ospray/ospray.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

//线程数量
#define NUM_THREADS 1

//日志模式
#define LOG_LEVEL 2

using namespace std;
using namespace osp;
using namespace cv;

void status_func(const char *message) {
    cout << message << endl;
}


int main(int argc, const char **argv) {
//    //创建设备
//    OSPDevice device;
//    device = ospNewDevice();

    ospInit(&argc, argv);

    OSPDevice device = ospGetCurrentDevice();

    //设置OSPRay初始化参数, debug模式等价于NUM_THREAD=1 && LOG_LEVEL=2
    ospDeviceSet1i(device, "numThreads", NUM_THREADS);
    ospDeviceSet1i(device, "logLevel", LOG_LEVEL);

    //OSPRay初始化完毕
    ospDeviceCommit(device);

    //使用设置好的device
    ospSetCurrentDevice(device);

    //设置状态回调函数
    ospDeviceSetStatusFunc(device, status_func);

    //加载Volume数据
    char filename[] = "assets/volume/csafe-heptane-302-volume.raw";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        cout << "cannot open file '" << filename << "' !" << endl;
        exit(1);
    }
    size_t volume_size = 302 * 302 * 302;
    unsigned char *buf = new unsigned char[volume_size];
    fread(buf, sizeof(unsigned char), volume_size, fp);

    OSPData data = ospNewData(302 * 302 * 302, OSP_UCHAR, buf, OSP_DATA_SHARED_BUFFER);
    ospCommit(data);

    OSPVolume volume = ospNewVolume("shared_structured_volume");
    ospSetString(volume, "voxelType", "uchar");
    vec3i dims = {302, 302, 302};
    vec2f range = {0.0f, 255.0f};
    ospSetVec2f(volume, "voxelRange", range);
    ospSetVec3i(volume, "dimensions", dims);
    ospSetData(volume, "voxelData", data);

    vec2f valueRange = {0.0f, 255.0f};
    vec3f colors[2] = {
            {0,   0,   0},
            {255, 255, 255}
    };
    float opacities[2] = {0.0f, 255.0f};

    OSPTransferFunction transferFunction = ospNewTransferFunction("piecewise_linear");
    OSPData colorData = ospNewData(2, OSP_FLOAT3A, colors);
    ospCommit(colorData);
    OSPData opacityData = ospNewData(2, OSP_FLOAT, opacities);
    ospCommit(opacityData);

    ospSetData(transferFunction, "colors", colorData);
    ospSetData(transferFunction, "opacities", opacityData);
    ospSetVec2f(transferFunction, "valueRange", valueRange);

    ospCommit(transferFunction);

    ospSetObject(volume, "transferFunction", transferFunction);

    ospCommit(volume);

    OSPModel world = ospNewModel();
    ospAddVolume(world, volume);
    ospCommit(world);

    //设置摄像头
    OSPCamera camera = ospNewCamera("perspective");
    ospSetf(camera, "aspect", 1.0f);
    ospCommit(camera);



    //设置Renderer
    OSPRenderer renderer = ospNewRenderer("raytracer");
    ospSetVec3f(renderer, "bgColor", osp::vec3f{255.0f, 255.0f, 255.0f});
    ospSetVec3f(renderer, "bgColor", osp::vec3f{0.0f, 0.0f, 0.0f});
    ospSetObject(renderer, "model", world);
    ospSetObject(renderer, "camera", camera);

//    OSPLight ambient_light = ospNewLight(renderer, "AmbientLight");
//    ospSet1f(ambient_light, "intensity", 0.0f);
//    ospCommit(ambient_light);
//
//    OSPLight directional_light = ospNewLight(renderer, "DirectionalLight");
//    ospSet1f(directional_light, "intensity", 2.0f);
//    ospSetVec3f(directional_light, "direction", osp::vec3f{20.0f, 20.0f, 20.0f});
//    ospCommit(directional_light);
//
//    std::vector<OSPLight> light_list{ambient_light, directional_light};
//    OSPData lights = ospNewData(light_list.size(), OSP_OBJECT, light_list.data());
//    ospCommit(lights);
//
//    ospSetData(renderer, "lights", lights);
    ospCommit(renderer);

    vec2i window_size;
    window_size.x = 256;
    window_size.y = 256;
    OSPFrameBuffer framebuffer = ospNewFrameBuffer(window_size, OSP_FB_RGBA8, OSP_FB_COLOR | OSP_FB_ACCUM);
    ospCommit(framebuffer);

    ospRenderFrame(framebuffer, renderer, OSP_FB_COLOR | OSP_FB_ACCUM);

    uint32_t *fdata = (uint32_t *) ospMapFrameBuffer(framebuffer, OSP_FB_COLOR);

    if (fdata == NULL) {
        cout << "Fdata is NULL" << endl;
    }

    QApplication app(argc, (char **) argv);

    QLabel label;
    QImage image(256, 256, QImage::Format_RGB888);
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            int r, g, b, a, pos;
            pos = (i * 256 + j) * 3;
            r = *(fdata + pos + 0);
            g = *(fdata + pos + 1);
            b = *(fdata + pos + 2);
            image.setPixel(i, j, qRgb(r, g, b));
        }
    }

    label.setPixmap(QPixmap::fromImage(image));

    label.show();

    return app.exec();
}