#include "DirLight.h"

 DirLight::DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) 
 :Light(ambient,diffuse,specular)
 ,m_direction(direction)
 {}



void DirLight::pack(LightsBlock& b, const glm::mat4& world) const {
    if (b.numDir >= MAX_DIR) return;
    auto& o = b.dir[b.numDir++];
    o.direction = glm::normalize(glm::mat3(world) * m_direction);
    o.ambient = m_ambient; o.diffuse = m_diffuse; o.specular = m_specular;
}