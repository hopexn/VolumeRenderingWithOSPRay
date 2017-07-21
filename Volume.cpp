#include "Volume.h"

MyVolume::MyVolume() {
    volume = ospNewVolume("shared_structured_volume");
    gridOrigin = {-0.5f, -0.5f, 0};
}

MyVolume::~MyVolume() {
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

    gridSpacing = {1.0f / dims.x, 1.0f / dims.y, 1.0f / dims.z};

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
    ospSetRegion(volume, buf, osp::vec3i{0, 0, 0}, dims);
    ospSetVec3f(volume, "gridOrigin", gridOrigin);
    ospSetVec3f(volume, "gridSpacing", gridSpacing);
    ospSetVec2f(volume, "voxelRange", osp::vec2f{0.0, 255.0f});
    ospSet1f(volume, "samplingRate", 100.0f);
    ospSetObject(volume, "transferFunction", tf1d.getTransferFunction());
    ospCommit(volume);
}

void MyVolume::setVoxelRange(const osp::vec2f &voxelRange) {
    MyVolume::voxelRange = voxelRange;
}

void MyVolume::setSamplingRate(float samplingRate) {
    MyVolume::samplingRate = samplingRate;
}
