#pragma once
#include "Transform.h"
#include <GLFW/glfw3.h>

class Rotation : public Transform{
    public:
        explicit Rotation(float angle, glm::vec3 axis, float speed = .0f) : angle(angle), axis(axis), speed(speed) {}
        glm::mat4 getMatrix() const override;
    private:
        float angle;
        glm::vec3 axis;
        float speed;
};
