#include "Camera.h"

MyCamera::MyCamera() {
    pos = {0, 0, 5};
    dir = {0, 0, -1};
    up = {0, 1, 0};

    nearClipping = 100;

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
    //ospSet1f(camera, "nearClipping", nearClipping);  todo
    ospCommit(camera);
}

