#pragma once
#include "Light.h"


class DirLight : public Light {
    public:
        DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
        void pack(LightsBlock& b, const glm::mat4& world) const override;

    private:
        glm::vec3 m_direction;


};