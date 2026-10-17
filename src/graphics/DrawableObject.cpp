/**
 * @file DrawableObject.cpp
 *
 * @brief DrawableProject Implementation
 *
 * @author KUC0438 
  **/
#include "DrawableObject.h"

DrawableObject::DrawableObject(std::shared_ptr<Model> model,std::shared_ptr<ShaderProgram> shader)
    : m_model(std::move(model))
    , m_shader(std::move(shader))
{}


DrawableObject::DrawableObject(std::shared_ptr<Model> model)
    : m_model(std::move(model))
    , m_shader(nullptr)
{}

void DrawableObject::update(float deltat){
    // TO be implemented
}

void DrawableObject::setTransform(std::shared_ptr<Transform> t){
    m_transform = std::move(t);
}

void DrawableObject::setTime(float time){
    m_shader->use();
    m_shader->setTime(time);
}

void DrawableObject::draw(ShaderProgram& fallbackShader){
    ShaderProgram& shader = m_shader ? *m_shader : fallbackShader;
    shader.use();
    glm::mat4 Matrice = m_transform ? m_transform->getMatrix() : glm::mat4(1.f); // If transform is nullptr, use identity matrix 
    shader.setMat4("modelMatrix", Matrice); 
     
    m_model->draw();
}
