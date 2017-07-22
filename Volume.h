#ifndef VOLUME_HPP
#define VOLUME_HPP

#include "ospray/ospray.h"
#include "common.h"
#include "TF1D.h"
#include <iostream>
#include <string>

class MyVolume {
public:
    MyVolume();

    ~MyVolume();

    void loadFromVifoFile(std::string filename);

    OSPVolume volume = NULL;

    void setup() {
        //设置Volume参数
        ospSetString(volume, "voxelType", "uchar");
        ospSetVec3i(volume, "dimensions", dims);
        ospSetRegion(volume, buf, osp::vec3i{0, 0, 0}, dims);
        ospSetVec3f(volume, "gridOrigin", gridOrigin);
        ospSetVec3f(volume, "gridSpacing", gridSpacing);
        ospSetVec2f(volume, "voxelRange", osp::vec2f{0.0, 255.0f});
        ospSet1f(volume, "samplingRate", samplingRate);
        ospSetObject(volume, "transferFunction", tf1d.tf);
        ospCommit(volume);
    }

    void setSamplingRate(float value) {
        samplingRate = value;
        setup();
    }

    void loadTF1D(std::string filename) {
        tf1d.loadFromFile(filename);
        setup();
    }

private:
    TF1D tf1d;

    osp::vec3i dims;
    osp::vec3i space;
    unsigned char *buf;

    osp::vec3f gridSpacing;
    osp::vec3f gridOrigin;

    float samplingRate;
};

#endif //VOLUME_HPP