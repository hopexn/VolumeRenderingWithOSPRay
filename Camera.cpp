#include "Camera.h"
#include "common.h"

MyCamera::MyCamera() {
    pos = {CAMERA_POSITION_X_INIT, CAMERA_POSITION_Y_INIT, CAMERA_POSITION_Z_INIT};
    dir = {CAMERA_DIRECTION_X_INIT, CAMERA_DIRECTION_Y_INIT, CAMERA_DIRECTION_Z_INIT};
    up = {CAMERA_UP_X_INIT, CAMERA_UP_Y_INIT, CAMERA_UP_Z_INIT};

    //透视模式
    camera = ospNewCamera("perspective");

    this->setup();
}

MyCamera::~MyCamera() {
    ospRelease(camera);
}


void MyCamera::setPos(const osp::vec3f &pos) {
    this->pos = pos;
}

void MyCamera::setDir(const osp::vec3f &dir) {
    this->dir = dir;
}

void MyCamera::setUp(const osp::vec3f &up) {
    this->up = up;
}

void MyCamera::setNearClipping(float nearClipping) {
    this->nearClipping = nearClipping;
}

OSPCamera MyCamera::getCamera() {
    return camera;
}

void MyCamera::setup() {
    ospSetVec3f(camera, "pos", pos);
    ospSetVec3f(camera, "dir", dir);
    ospSetVec3f(camera, "up", up);
    ospCommit(camera);
}

