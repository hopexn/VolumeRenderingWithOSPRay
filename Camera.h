#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include "ospray/ospray.h"
#include "common.h"
#include <OpenGL.h>
#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/fast_square_root.hpp>

using namespace std;

class MyCamera {
public:
    MyCamera();

    ~MyCamera();

    OSPCamera getCamera();

    void setup();

    void rotate(osp::vec3f rotation) {
        if (rotation.x == 0 && rotation.y == 0) {
            return;
        }

        glm::vec3 axis = glm::fastNormalize(glm::cross(glm::vec3(0, 0, 1), glm::vec3(rotation.x, rotation.y, 0)));
        int sign = rotation.x * rotation.y > 0 ? 1 : -1;
        float theta = (float) (glm::sqrt(glm::pow(rotation.x, 2) + glm::pow(rotation.y, 2)));

        float u = axis.x;
        float v = axis.y;
        float w = axis.z;
        float m00, m01, m02, m10, m11, m12, m20, m21, m22;

        m00 = cosf(theta) + (u * u) * (1 - cosf(theta));
        m01 = u * v * (1 - cosf(theta)) + w * sinf(theta);
        m02 = u * w * (1 - cosf(theta)) - v * sinf(theta);

        m10 = u * v * (1 - cosf(theta)) - w * sinf(theta);
        m11 = cosf(theta) + v * v * (1 - cosf(theta));
        m12 = w * v * (1 - cosf(theta)) + u * sinf(theta);

        m20 = u * w * (1 - cosf(theta)) + v * sinf(theta);
        m21 = v * w * (1 - cosf(theta)) - u * sinf(theta);
        m22 = cosf(theta) + w * w * (1 - cosf(theta));

        glm::vec3 cam_pos, cam_up, cam_dir;
        glm::vec3 cam_pos_init(CAMERA_POSITION_X_INIT, CAMERA_POSITION_Z_INIT, CAMERA_POSITION_Z_INIT),
                cam_up_init(CAMERA_UP_X_INIT, CAMERA_UP_Y_INIT, CAMERA_UP_Z_INIT);
        glm::vec3 center(0, 0, 0);
        cam_pos.x = cam_pos_init.x * m00 + cam_pos_init.y * m01 + cam_pos_init.z * m02;
        cam_pos.y = cam_pos_init.x * m10 + cam_pos_init.y * m11 + cam_pos_init.z * m12;
        cam_pos.z = cam_pos_init.x * m20 + cam_pos_init.y * m21 + cam_pos_init.z * m22;

        cam_up.x = cam_up_init.x * m00 + cam_up_init.y * m01 + cam_up_init.z * m02;
        cam_up.y = cam_up_init.x * m10 + cam_up_init.y * m11 + cam_up_init.z * m12;
        cam_up.z = cam_up_init.x * m20 + cam_up_init.y * m21 + cam_up_init.z * m22;

        cam_dir = fastNormalize(center - cam_pos);

        pos.x = cam_pos.x;
        pos.y = cam_pos.y;
        pos.z = cam_pos.z;

        dir.x = cam_dir.x;
        dir.y = cam_dir.y;
        dir.z = cam_dir.z;

        up.x = cam_up.x;
        up.y = cam_up.y;
        up.z = cam_up.z;

        double dist = glm::pow(cam_pos.x, 2) + glm::pow(cam_pos.y, 2) + glm::pow(cam_pos.z, 2);
        cout << "cam_pos:" << cam_pos.x << " " << cam_pos.y << " " << cam_pos.z << " " << dist << endl;
        cout << "cam_dir:" << cam_dir.x << " " << cam_dir.y << " " << cam_dir.z << endl;
        cout << "cam_up:" << cam_up.x << " " << cam_up.y << " " << cam_up.z << endl;
        cout << "rotate:" << rotation.x << " " << rotation.y << " " << rotation.z << endl;
        setup();
    }

private:
    OSPCamera camera = NULL;

    //摄像头的位置、朝向
    osp::vec3f pos, dir, up;

    //近端截取
    float nearClipping;


};

#endif //CAMERA_HPP
