#ifndef TF1D_HPP
#define TF1D_HPP
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
        float index, opacity;
        osp::vec3f color;

        //清除vector中的内容
        colors.clear();
        opacities.clear();
        indexs.clear();

        FILE *fp = fopen(filename.c_str(), "r");
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
        fclose(fp);
    }

    void writeToFile(std::string filename) {
        FILE *fp = NULL;
        float index, opacity;
        osp::vec3f color;

        fp = fopen(filename.c_str(), "w");
        if (fp == NULL) {
            std::cout << "Cannot open file '" << filename << "' !" << std::endl;
            return;
        }

        fprintf(fp, "%d %f %f\n", num, valueRange.x, valueRange.y);
        for (int i = 0; i < num; i++) {
            index = indexs[i];
            color = colors[i];
            opacity = opacities[i];
            fprintf(fp, "%f %f %f %f %f %f %f %f %f",
                    index, color.x, color.y, color.z, opacity,
                    color.x, color.y, color.z, opacity);
        }
        fclose(fp);
    }

    void setup() {
        int item_num = 256;

        float ratio_front, ratio_rear;
        float index, index_front, index_rear;
        float opacity, opacity_front, opacity_rear;
        osp::vec3f color, color_front, color_rear;

        std::vector<osp::vec3f> colors_tmp;
        std::vector<float> opacities_tmp;

        int curr = 0;
        for (int i = 0; i < item_num; i++) {
            index = valueRange.x + (float) i / item_num * (valueRange.y - valueRange.x);
            index_front = indexs[curr];
            index_rear = curr < num - 1 ? indexs[curr + 1] : valueRange.y;
            if (index > index_rear) {
                curr++;
                continue;
            }
            color_front = colors[curr];
            color_rear = colors[curr < num - 1 ? curr + 1 : 0];
            opacity_front = opacities[curr];
            opacity_rear = opacities[curr < num - 1 ? curr + 1 : 0];

            ratio_front = (index - index_front) / (index_rear - index_front);
            ratio_rear = 1 - ratio_front;

            color.x = ratio_front * color_rear.x + ratio_rear * color_front.x;
            color.y = ratio_front * color_rear.y + ratio_rear * color_front.y;
            color.z = ratio_front * color_rear.z + ratio_rear * color_front.z;
            opacity = ratio_front * opacity_rear + ratio_rear * opacity_front;

            colors_tmp.push_back(color);
            opacities_tmp.push_back(opacity);
        }

        OSPData colors_data = ospNewData((size_t) item_num, OSP_FLOAT3, colors_tmp.data());
        ospCommit(colors_data);
        OSPData opacities_data = ospNewData((size_t) item_num, OSP_FLOAT, opacities_tmp.data());
        ospCommit(opacities_data);

        ospRelease(tf);
        tf = ospNewTransferFunction("piecewise_linear");
        ospSetData(tf, "colors", colors_data);
        ospSetData(tf, "opacities", opacities_data);
        ospCommit(tf);
    }

    static TF1D &getInstance() {
        return tf1d;
    }

    OSPTransferFunction getTransferFunctionD() {
        return tf;
    }

private:
    static TF1D tf1d;

    OSPTransferFunction tf = NULL;

    int num;
    std::vector<osp::vec3f> colors;
    std::vector<float> indexs;
    std::vector<float> opacities;
    osp::vec2f valueRange;
};

#endif