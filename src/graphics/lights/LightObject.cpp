#include "LightObject.h"
#include "../../core/Logger.hpp"
#include "../../core/Camera.h"
#include "SpotLight.h"

LightObject::LightObject(Model& model, ShaderProgram& emissiveShader,unsigned int id, std::shared_ptr<Light> light)
    : DrawableObject(model, emissiveShader,id)
    , m_light(std::move(light)) 
{}

void LightObject::pack(LightsBlock& b){
    glm::mat4 matrice = m_transform ? m_transform->getMatrix() : glm::mat4(1.f);
    matrice[3] = glm::vec4(getPos(),1.0f);
    m_light->pack(b, matrice); 
}

void LightObject::pack(LightsBlock& b, Camera& c){
    if (auto spot = std::dynamic_pointer_cast<SpotLight>(m_light)) {
        spot->setPose(c.getPos(), c.getFront());
        spot->pack(b, glm::mat4(1.0f));
        return;
    }
    glm::mat4 matrice = m_transform ? m_transform->getMatrix() : glm::mat4(1.f);
    matrice[3] = glm::vec4(getPos(),1.0f);
    m_light->pack(b, matrice); 
}

void LightObject::enable(){
       if (auto spot = std::dynamic_pointer_cast<SpotLight>(m_light)) {
        spot->enable();
    }
}