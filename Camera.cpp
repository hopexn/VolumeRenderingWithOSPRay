#include "Camera.h"

MyCamera::MyCamera() {
    pos = {CAMERA_POSITION_X_INIT, CAMERA_POSITION_Y_INIT, CAMERA_POSITION_Z_INIT};
    dir = {CAMERA_DIRECTION_X_INIT, CAMERA_DIRECTION_Y_INIT, CAMERA_DIRECTION_Z_INIT};
    up = {CAMERA_UP_X_INIT, CAMERA_UP_Y_INIT, CAMERA_UP_Z_INIT};

    //透视模式
    camera = ospNewCamera("perspective");

    this->rotate(osp::vec3f{0, 0, 0});
    this->setup();
}

MyCamera::~MyCamera() {
    ospRelease(camera);
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

