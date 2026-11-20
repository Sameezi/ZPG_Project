#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
: Light(ambient, diffuse, specular)
, m_position(position)
, m_constant(constant)
, m_linear(linear)
, m_quadratic(quadratic)
{}

void PointLight::pack(LightsBlock& b, const glm::mat4& world) const {
        if (b.numPoint >= MAX_POINT) return;
        auto& o = b.point[b.numPoint++];
        o.position = glm::vec3(world * glm::vec4(0, 0, 0, 1));
        o.constant = m_constant; o.linear = m_linear; o.quadratic = m_quadratic;
        o.ambient = m_ambient; o.diffuse = m_diffuse; o.specular = m_specular;
}