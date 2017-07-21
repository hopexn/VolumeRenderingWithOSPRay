#include "Volume.h"

MyVolume::MyVolume() {
    volume = ospNewVolume("block_bricked_volume");
    gridOrigin = {-0.5f, -0.5f, 0};
}

MyVolume::~MyVolume() {
    ospRelease(volume);
}

void MyVolume::loadFromVifoFile(std::string filename) {
    FILE *fp;
    char temp[MAX_BUFFER_SIZE];

    //从vifo文件中获取体数据信息
    fp = fopen(filename.c_str(), "r");
    if (fp == NULL) {
        std::cout << "Can't open file '" << filename << "' !" << std::endl;
        return;
    }
    fscanf(fp, "%d %d %d\n", &dims.x, &dims.y, &dims.z);
    fscanf(fp, "%d %d %d\n", &space.x, &space.y, &space.z);
    fscanf(fp, "%s", temp);
    fclose(fp);

    //加载raw文件
    std::string volume_file = filename.substr(0, filename.rfind('/') + 1);
    volume_file += temp;
    fp = fopen(volume_file.c_str(), "r");
    if (fp == NULL) {
        std::cout << "Cannot open file '" << filename << "' !" << std::endl;
        return;
    }

    size_t size = (size_t) dims.x * dims.y * dims.z;
    buf = new unsigned char[size];
    fread(buf, sizeof(unsigned char), size, fp);
    fclose(fp);

    //根据维度确定步长
    gridSpacing = {1.0f / dims.x, 1.0f / dims.y, 1.0f / dims.z};

    std::string tf_file = volume_file.substr(0, volume_file.rfind('.') + 1);
    tf_file += "TF1D";
    tf1d.loadFromFile(tf_file);

    //设置Volume参数
    ospSetString(volume, "voxelType", "uchar");
    ospSetVec3i(volume, "dimensions", dims);
    ospSetRegion(volume, buf, osp::vec3i{0, 0, 0}, dims);
    ospSetVec3f(volume, "gridOrigin", gridOrigin);
    ospSetVec3f(volume, "gridSpacing", gridSpacing);
    ospSetVec2f(volume, "voxelRange", osp::vec2f{0.0, 255.0f});
    ospSet1f(volume, "samplingRate", VOLUME_SAMPLINGRATE_INIT);
    ospSetObject(volume, "transferFunction", tf1d.tf);
    ospCommit(volume);
}
