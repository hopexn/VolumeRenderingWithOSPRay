#include "Volume.h"

MyVolume::MyVolume() {
    volume = ospNewVolume("block_bricked_volume");
    samplingRate = VOLUME_SAMPLINGRATE_INIT;
    specularRate = {RENDER_SPECULAR_RATE_INIT, RENDER_SPECULAR_RATE_INIT, RENDER_SPECULAR_RATE_INIT};
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


    int max_dim = std::max(dims.x, std::max(dims.y, dims.z));
    float xfSize = (float) dims.x / max_dim;
    float yfSize = (float) dims.y / max_dim;
    float zfSize = (float) dims.z / max_dim;

    gridOrigin = {-0.5f * xfSize, -0.5f * yfSize, -0.5f * zfSize};

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
    gridSpacing = {xfSize / dims.x, yfSize / dims.y, zfSize / dims.z};

    std::string tf_file = volume_file.substr(0, volume_file.rfind('.') + 1);
    tf_file += "TF1D";
    tf1d.loadFromFile(tf_file);

    setup();
}
