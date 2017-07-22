#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include "ospray/ospray.h"
#include "common.h"
#include <OpenGL.h>
#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/fast_square_root.hpp>
#include "util/Vector.h"

class MyCamera {
public:
    MyCamera();

    ~MyCamera();

    void update();

    OSPCamera camera = NULL;

    //摄像头的位置、朝向
    Vector3f pos, dir, up, center;

    //近端截取
    float nearClipping;

    void rotate(float dx, float dy) {
        float theta = 3.14f * (std::fabs(dx) + std::fabs(dy));
        Vector3f right = Cross(dir, up).normalize();
        Vector3f vec = up * dy + right * dx;
        if (dx == 0 && dy == 0) return;

        Vector3f rotation_axis = Cross(vec, -dir);
        if (rotation_axis.zero()) return;
        rotation_axis = rotation_axis.normalize();
        pos = Rotate(pos, center, rotation_axis, theta);
        dir = Rotate(dir, rotation_axis, theta).normalize();
        up = Rotate(up, rotation_axis, theta).normalize();
#ifdef DEBUG
        std::cout << "rotation" << std::endl;
        std::cout << "pos" << pos << std::endl;
        std::cout << "dir" << dir << std::endl;
        std::cout << "up" << up << std::endl;
#endif
        update();
    }

    void scale(float s) {
        pos = center + (pos - center) * s;
#ifdef DEBUG
        std::cout << "scale" << std::endl;
        std::cout << "pos" << pos << std::endl;
        std::cout << "dir" << dir << std::endl;
        std::cout << "up" << up << std::endl;
#endif
        update();
    }

    void translate(float dx, float dy) {
        Vector3f right = Cross(dir, up);
        Vector3f translation = ((right * dx) + (up * dy));
        center += translation;
        pos += translation;
#ifdef DEBUG
        std::cout << "translation" << std::endl;
        std::cout << "pos" << pos << std::endl;
        std::cout << "dir" << dir << std::endl;
        std::cout << "up" << up << std::endl;
#endif
        update();
    }

    osp::vec3f osp_vec(Vector3f vec) {
        return osp::vec3f{vec.x, vec.y, vec.z};
    }
};

#endif //CAMERA_HPP
