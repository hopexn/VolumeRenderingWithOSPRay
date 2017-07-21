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

    TF1D tf1d;

    OSPVolume volume = NULL;

private:
    osp::vec3i dims;
    osp::vec3i space;
    unsigned char *buf;

    osp::vec3f gridSpacing;
    osp::vec3f gridOrigin;

    float samplingRate;
};

#endif //VOLUME_HPP