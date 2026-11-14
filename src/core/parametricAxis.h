#pragma once
#include <glm/glm.hpp>

struct parametricAxis{
    glm::vec3 A = glm::vec3(0.0f);
    glm::vec3 B = glm::vec3(0.0f);

    float t = 0.0f;
    float time = 0.0f;
    int index = 0;
};