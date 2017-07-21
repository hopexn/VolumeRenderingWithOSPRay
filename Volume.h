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

    osp::Volume *getVolume();
private:
    OSPVolume volume = NULL;

private:
    osp::vec3i dims;
    osp::vec3i space;
    unsigned char *buf;

    osp::vec3f gridSpacing;
    osp::vec3f gridOrigin;

    TF1D tf1d;

    float samplingRate;

    void loadRawFile(std::string filename);

    void setup();
};

#endif //VOLUME_HPP