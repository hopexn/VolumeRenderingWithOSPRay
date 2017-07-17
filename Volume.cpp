#include "Volume.h"
MyVolume::MyVolume() {
    volume = ospNewVolume("shared_structured_volume");
}

~MyVolume::MyVolume() {
    ospRelease(volume);
}

void MyVolume::loadFromVifoFile(std::string filename) {
    FILE *fp = fopen(filename.c_str(), "r");
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

OSPVolume MyVolume::getVolume() {
    return volume;
}


void MyVolume::loadRawFile(std::string filename) {
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

void MyVolume::setup() {
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
    ospSetObject(volume, "transferFunction", tf1d.getTransferFunction());
    ospCommit(volume);
}
