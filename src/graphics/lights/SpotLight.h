#pragma once
#include "PointLight.h"


class SpotLight : public PointLight{
    public:
        SpotLight(glm::vec3 position, glm::vec3 direction,glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff);
        void pack(LightsBlock& b, const glm::mat4& world) const override;
    private:
        glm::vec3 m_direction;
        float m_cutOff, m_outerCutOff;
};