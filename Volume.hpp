#ifndef VOLUME_HPP
#define VOLUME_HPP

#include "ospray/ospray.h"
#include "common.h"
#include "TF1D.hpp"
#include <iostream>
#include <string>


class Volume {
public:
    Volume() {
        volume = ospNewVolume("shared_structured_volume");
        tf1d = TF1D::getInstance();
    }

    ~Volume() {
        ospRelease(volume);
    }

    void loadFromVifoFile(std::string filename) {
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            std::cout << "Can't open file '" << filename << "' !" << std::endl;
            return;
        }

        // read volume information
        char dataFile[1024];
        fscanf(fp, "%d %d %d\n", &dims.x, &dims.y, &dims.z);
        fscanf(fp, "%d %d %d\n", &space.x, &space.y, &space.z);
        fscanf(fp, "%s", dataFile);

        fclose(fp);

        std::string filePath(filename);
        filePath = filePath.substr(0, filePath.rfind('/') + 1);
        filePath += dataFile;
        loadRawFile(filePath);

        std::string tfFile = filePath.substr(0, filePath.rfind('.') + 1);
        tfFile += "TF1D";
        tf1d.loadFromFile(tfFile);

        setup();
    }


    void setup() {
        ospSetString(volume, "voxelType", "uchar");
        ospSetVec3i(volume, "dimensions", dims);

        size_t size = (size_t) dims.x * dims.y * dims.z;
        OSPData data = ospNewData(size, OSP_UCHAR, buf);
        ospCommit(data);

        ospSetVec3f(volume, "gridOrigin", gridOrigin);
        ospSetVec3f(volume, "gridSpacing", gridSpacing);
        ospSetVec2f(volume, "voxelRange", voxelRange);
        ospSetData(volume, "voxelData", data);
        ospSet1f(volume, "samplingRate", samplingRate);
        ospSetObject(volume, "transferFunction", tf1d.getTransferFunctionD());
        ospCommit(volume);
    }

private:
    OSPVolume volume = NULL;
    osp::vec3i dims;
    osp::vec3i space;
    unsigned char *buf;

    osp::vec2f voxelRange;
    osp::vec3f gridSpacing;
    osp::vec3f gridOrigin;

    TF1D tf1d;
    float samplingRate = 0.125f;

    void loadRawFile(std::string filename) {
        FILE *fp = fopen(filename.c_str(), "r");
        if (fp == NULL) {
            std::cout << "Cannot open file '" << filename << "' !" << std::endl;
            return;
        }

        delete[] buf;
        size_t size = (size_t) dims.x * dims.y * dims.z;
        buf = new unsigned char[size];
        fread(buf, sizeof(unsigned char), size, fp);
    }
};

#endif //VOLUME_HPP