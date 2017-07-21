#include "Camera.h"

MyCamera::MyCamera() {
    pos = Vector3f(CAMERA_POSITION_X_INIT, CAMERA_POSITION_Y_INIT, CAMERA_POSITION_Z_INIT);
    dir = Vector3f(CAMERA_DIRECTION_X_INIT, CAMERA_DIRECTION_Y_INIT, CAMERA_DIRECTION_Z_INIT);
    up = Vector3f(CAMERA_UP_X_INIT, CAMERA_UP_Y_INIT, CAMERA_UP_Z_INIT);
    center = Vector3f(0, 0, 0);

    //透视模式
    camera = ospNewCamera("perspective");

    this->update();
}

MyCamera::~MyCamera() {
    ospRelease(camera);
}

void MyCamera::update() {
    ospSetVec3f(camera, "pos", osp_vec(pos));
    ospSetVec3f(camera, "dir", osp_vec(dir));
    ospSetVec3f(camera, "up", osp_vec(up));
    ospCommit(camera);
}

