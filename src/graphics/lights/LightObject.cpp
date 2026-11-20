#include "LightObject.h"
#include "../../core/Logger.hpp"

LightObject::LightObject(Model& model, ShaderProgram& emissiveShader,unsigned int id, std::shared_ptr<Light> light)
    : DrawableObject(model, emissiveShader,id)
    , m_light(std::move(light)) 
{}

void LightObject::pack(LightsBlock& b){
    m_light->pack(b,  m_transform ? m_transform->getMatrix() : glm::mat4(1.f));
}