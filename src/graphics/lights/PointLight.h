#pragma once
#include "Light.h"



class PointLight : public Light{
    public:
        PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
        void pack(LightsBlock& b, const glm::mat4& world) const override;

    protected:
        glm::vec3 m_position;
        float m_constant, m_linear, m_quadratic;
};