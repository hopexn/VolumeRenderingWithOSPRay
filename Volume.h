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

    OSPVolume getVolume();

private:
    OSPVolume volume = NULL;
    osp::vec3i dims;
    osp::vec3i space;
    unsigned char *buf;

    osp::vec2f voxelRange;
    osp::vec3f gridSpacing;
    osp::vec3f gridOrigin;

    static TF1D tf1d;
    float samplingRate = 0.125f;

    void loadRawFile(std::string filename);

    void setup();
};

#endif //VOLUME_HPP