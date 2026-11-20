#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../ShaderProgram.h"
#include "Ubo.h"


class Light {
public:
    virtual ~Light() = default;
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular){}
    virtual void pack(LightsBlock& block, const glm::mat4& world) const = 0;

    glm::vec3 m_ambient{0.05f}, m_diffuse{0.4f}, m_specular{0.5f};
};
