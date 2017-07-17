#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ospray/ospray.h"

class Camera {
public:
    Camera() {
        pos = {0, 0, 5};
        dir = {0, 0, -1};
        up = {0, 1, 0};


        nearClipping = 100;

        //透视模式
        camera = ospNewCamera("perspective");

        this->setup();
    }

    ~Camera() {
        ospRelease(camera);
    }

    void setup() {
        ospSetVec3f(camera, "pos", pos);
        ospSetVec3f(camera, "dir", dir);
        ospSetVec3f(camera, "up", up);
        //ospSet1f(camera, "nearClipping", nearClipping);  todo
        ospCommit(camera);
    }

    void setPos(const osp::vec3f &pos) {
        Camera::pos = pos;
    }

    void setDir(const osp::vec3f &dir) {
        Camera::dir = dir;
    }

    void setUp(const osp::vec3f &up) {
        Camera::up = up;
    }

    void setNearClipping(float nearClipping) {
        Camera::nearClipping = nearClipping;
    }

private:

    static OSPCamera camera = NULL;

    //摄像头的位置、朝向
    osp::vec3f pos, dir, up;

    //近端截取
    float nearClipping;
};

#endif //CAMERA_HPP
