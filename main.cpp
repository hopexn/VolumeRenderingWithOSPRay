//C++
#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QImage>
#include <QPixmap>

//C
#include "ospray/ospray.h"
#include <opencv2/opencv.hpp>

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

void writePPM(const char *fileName, const vec2i *size, const uint32_t *pixel) {
    FILE *file = fopen(fileName, "wb");
    fprintf(file, "P6\n%i %i\n255\n", size->x, size->y);
    unsigned char *out = (unsigned char *) alloca(3 * size->x);
    for (int y = 0; y < size->y; y++) {
        const unsigned char *in = (const unsigned char *) &pixel[(size->y - 1 - y) * size->x];
        for (int x = 0; x < size->x; x++) {
            out[3 * x + 0] = in[4 * x + 0];
            out[3 * x + 1] = in[4 * x + 1];
            out[3 * x + 2] = in[4 * x + 2];
        }
        fwrite(out, 3 * size->x, sizeof(char), file);
    }
    fprintf(file, "\n");
    fclose(file);
}


int main(int argc, const char **argv) {
//    //创建设备
//    OSPDevice device;
//    device = ospNewDevice();
    vec2i imgSize;
    imgSize.x = 512; // width
    imgSize.y = 512; // height

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
    char filename[] = "assets/volume/engine.raw";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        cout << "cannot open file '" << filename << "' !" << endl;
        exit(1);
    }
    size_t volume_size = 256 * 256 * 128;
    unsigned char *buf = new unsigned char[volume_size];
    fread(buf, sizeof(unsigned char), volume_size, fp);

    OSPData volume_data = ospNewData(volume_size, OSP_UCHAR, buf, OSP_DATA_SHARED_BUFFER);
    ospCommit(volume_data);



    vec2f valueRange = {0.0f, 255.0f};
    vec3f colors[8] = {
            {0,   0,   0},
            {52,  52,  52},
            {170, 0,   0},
            {0,   170, 127},
            {0,   170, 127},
            {166, 111, 84},
            {255, 85,  0},
            {255, 0,   0}
    };
    float opacities[8] = {0, 0, 0, 0.05, 0, 0, 0.3};


    OSPTransferFunction transferFunction = ospNewTransferFunction("piecewise_linear");
    OSPData colorData = ospNewData(8, OSP_FLOAT3A, colors);
    ospCommit(colorData);
    OSPData opacityData = ospNewData(8, OSP_FLOAT, opacities);
    ospCommit(opacityData);

    ospSetVec2f(transferFunction, "valueRange", valueRange);
    ospSetData(transferFunction, "colors", colorData);
    ospSetData(transferFunction, "opacities", opacityData);

    ospCommit(transferFunction);


    OSPVolume volume = ospNewVolume("shared_structured_volume");
    ospSetString(volume, "voxelType", "uchar");
    int dims[] = {256, 256, 128};
    float voxelRange[] = {0.0f, 255.0f};
    float gridSpacing[] = {1.0f, 1.0f, 1.0f};
    float gridOrigin[] = {-0.5f, -0.5f, 10.5f};

    ospSet3fv(volume, "gridOrigin", gridOrigin);
    ospSet3fv(volume, "gridSpacing", gridSpacing);

    ospSet2fv(volume, "voxelRange", voxelRange);
    ospSet3iv(volume, "dimensions", dims);
    ospSetData(volume, "voxelData", volume_data);
    ospSet1f(volume, "samplingRate", .07f);
    ospSetObject(volume, "transferFunction", transferFunction);
    ospCommit(volume);

    OSPModel world = ospNewModel();
    ospAddVolume(world, volume);
    ospCommit(world);


    // camera
    float cam_pos[] = {0, 0, 1};
    float cam_up[] = {0, 1, 0};
    float cam_view[] = {0, 0, 1};
    //设置摄像头
    OSPCamera camera = ospNewCamera("orthographic");
    ospSetf(camera, "aspect", 1.0f);
    ospSetf(camera, "height", 500);
    ospSet3fv(camera, "pos", cam_pos);
    ospSet3fv(camera, "dir", cam_view);
    ospSet3fv(camera, "up", cam_up);
    ospCommit(camera); // commit each object to indicate modifications are done

    //设置Renderer
    OSPRenderer renderer = ospNewRenderer("scivis");
    ospSetObject(renderer, "model", world);
    ospSetObject(renderer, "camera", camera);


    OSPLight light = ospNewLight(renderer, "ambient");
    ospCommit(light);
    OSPData lights = ospNewData(1, OSP_LIGHT, &light, 0);
    ospCommit(lights);
    ospSetData(renderer, "lights", lights);
    ospCommit(renderer);

    OSPFrameBuffer framebuffer = ospNewFrameBuffer(imgSize, OSP_FB_SRGBA, OSP_FB_COLOR | OSP_FB_ACCUM);
    ospCommit(framebuffer);

    ospFrameBufferClear(framebuffer, OSP_FB_COLOR | OSP_FB_ACCUM);
    ospRenderFrame(framebuffer, renderer, OSP_FB_COLOR | OSP_FB_ACCUM);

    uint32_t *fdata = (uint32_t *) ospMapFrameBuffer(framebuffer, OSP_FB_COLOR);

    writePPM("test.ppm", &imgSize, fdata);

    return 0;
}