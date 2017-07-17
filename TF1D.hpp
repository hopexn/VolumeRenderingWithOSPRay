#ifndef TF1D_HPP

//C++ libraries
#include <iostream>
#include <string>
#include <vector>

//C libraries
#include "ospray/ospray.h"
#include <cstdio>
#include <cstdlib>


class TF1D {
public:
    TF1D() {}

    void loadFromFile(std::string filename) {
        FILE *fp = NULL;
        float index, opacity;
        osp::vec3f color;

        //清除vector中的内容
        colors.clear();
        opacities.clear();
        indexs.clear();

        fp = fopen(filename.c_str(), "r");
        if (fp == NULL) {
            std::cout << "Cannot open file '" << filename << "' !" << std::endl;
            return;
        }

        //读取文件内容，保存在vector中
        fscanf(fp, "%d %f %f", &num, &valueRange.x, &valueRange.y);
        if (valueRange.x >= valueRange.y) {
            std::cout << "Value range error !" << std::endl;
            return;
        }

        for (int i = 0; i < num; i++) {
            fscanf(fp, "%f %f %f %f %f %f %f %f %f",
                   &index, &color.x, &color.y, &color.z, &opacity,
                   &color.x, &color.y, &color.z, &opacity);
            colors.push_back(color);
            opacities.push_back(opacity);
            indexs.push_back(index);
        }

        this->setup();
    }

    void writeToFile(std::string filename) {

    }

    void setup() {
        float opacity;
        osp::vec3f color_front, color_rear;
        float index, index_front, index_rear;

        int item_num = 256;
        int steps;

        ospRelease(tf1d);
        tf1d = ospNewTransferFunction("piecewise_linear");
        osp::vec3f colors_tmp[item_num];
        osp::vec3f opacities_tmp[item_num];

        int curr = 0;
        for (int i = 0; i < item_num; i++) {
            index = (float) i / item_num * (valueRange.y - valueRange.x);
            index_front = indexs[curr];
            index_rear = curr == num - 1 ? valueRange.y : indexs[curr + 1];
            color_front = colors[curr];
            color
        }

        colors.clear();

    }

private:

    OSPTransferFunction tf1d = NULL;

    int num;
    std::vector<osp::vec3f> colors;
    std::vector<float> indexs;
    std::vector<float> opacities;
    osp::vec2f valueRange;
};

#endif