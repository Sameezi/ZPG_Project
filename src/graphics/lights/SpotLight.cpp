#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 position,glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
: PointLight(position, ambient, diffuse , specular, constant, linear, quadratic)
, m_direction(direction)
, m_cutOff(cutOff)
, m_outerCutOff(outerCutOff)
{}


void SpotLight::setPose(const glm::vec3& pos, const glm::vec3& front) {
     m_position = pos; 
     m_direction = front; 
}

void SpotLight::pack(LightsBlock& b, const glm::mat4& world) const{
    if (b.numSpot >= MAX_SPOT) return;
        auto& o = b.spot[b.numSpot++];
        o.position = glm::vec3(world * glm::vec4(m_position, 1.0f));
        o.enabled = m_enabled;
        o.direction = glm::normalize(glm::mat3(world) * m_direction);
        o.constant = m_constant; o.linear = m_linear; o.quadratic = m_quadratic;
        o.ambient = m_ambient; o.diffuse = m_diffuse; o.specular = m_specular;
        o.cutOff = m_cutOff; o.outerCutOff = m_outerCutOff;
}

void SpotLight::enable(){
    if(m_enabled == 0){
        m_enabled = 1;
    }else{
        m_enabled = 0;
    }
}
