#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ospray/ospray.h"

class MyCamera {
public:
    MyCamera();

    ~MyCamera();

    void setPos(const osp::vec3f &pos);

    void setDir(const osp::vec3f &dir);

    void setUp(const osp::vec3f &up);

    void setNearClipping(float nearClipping);

    OSPCamera getCamera();

    void setup();
private:
    OSPCamera camera = NULL;

    //摄像头的位置、朝向
    osp::vec3f pos, dir, up;

    //近端截取
    float nearClipping;


};

#endif //CAMERA_HPP
