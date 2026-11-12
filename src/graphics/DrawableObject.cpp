/**
 * @file DrawableObject.cpp
 *
 * @brief DrawableProject Implementation
 *
 * @author KUC0438 
  **/
#include "DrawableObject.h"

DrawableObject::DrawableObject(Model& model, ShaderProgram& shader, unsigned int id);
    : m_model(&model)
    , m_shader(&shader)
    , m_id(id)
{}

DrawableObject::DrawableObject(Model& model, unsigned int id);
    : m_model(&model)
    , m_shader(nullptr)
    , m_id(id)
{}

void DrawableObject::update(float deltat) {
    // TO be implemented
}

void DrawableObject::setTransform(std::shared_ptr<Transform> t) {
    m_transform = std::move(t);
}

void DrawableObject::setTime(float time) {
    if (!m_shader) return;   
    m_shader->use();
    m_shader->setTime(time);
}

void DrawableObject::draw(ShaderProgram& fallbackShader) {
    ShaderProgram& shader = m_shader ? *m_shader : fallbackShader;
    shader.use();

    glm::mat4 matrice = m_transform ? m_transform->getMatrix() : glm::mat4(1.f);

    if (!dynamic_cast<InstancedModel*>(m_model))
        shader.setMat4("modelMatrix", matrice);
    if(m_model->getTextureCount() > 1)
        for(int i = 0; i < m_model->getTextureCount(); i++)
            shader.setInt("textureUnit" + std::to_string(i+1), i);

    glStencilFunc(GL_ALWAYS, m_id, 0xFF);
    m_model->draw();
}

glm::vec3 DrawableObject::getPos() const {
    glm::mat4 m = m_transform ? m_transform->getMatrix() : glm::mat4(1.f);
    return glm::vec3(m[3]);
}
