//C++
#include <iostream>

using namespace std;

//C
#include "ospray/ospray.h"

//线程数量
#define NUM_THREADS 1

//日志模式
#define LOG_LEVEL 2

void status_func(const char *message) {
    cout << message << endl;
}

int main(int argc, const char **argv) {
    OSPDevice device;
    device = ospNewDevice("defualt");

    //设置OSPRay初始化参数, debug模式等价于NUM_THREAD=1 && LOG_LEVEL=2
    ospDeviceSet1i(device, "numThreads", NUM_THREADS);
    ospDeviceSet1i(device, "logLevel", LOG_LEVEL);

    //OSPRay初始化完毕
    ospDeviceCommit(device);

    //使用设置好的device
    ospSetCurrentDevice(device);

    //设置状态回调函数
    ospDeviceSetStatusFunc(device, status_func);




    std::cout << "Hello, World!" << std::endl;
    return 0;
}