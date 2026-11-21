#pragma once
#include <string>
#include <glm/glm.hpp>

struct Material {
    glm::vec3 ambient{1.0f};
    glm::vec3 diffuse{1.0f};
    glm::vec3 specular{1.0f};
    float shininess = 32.0f;

    static Material getMaterial(const std::string& name);
};