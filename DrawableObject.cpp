/**
 * @file DrawableObject.cpp
 *
 * @brief DrawableProject Implementation
 *
 * @author KUC0438 
  **/
#include "DrawableObject.h"

DrawableObject::DrawableObject(const std::vector<float>& vertices,std::unique_ptr<ShaderProgram> shader)
    : m_model(vertices)
    , m_ownedShader(std::move(shader))
{}


DrawableObject::DrawableObject(const std::vector<float>& vertices)
    : m_model(vertices)
    , m_ownedShader(nullptr)
{}

DrawableObject::DrawableObject(const std::vector<float>& vertices, int numFloats, std::unique_ptr<ShaderProgram> shader)
    : m_model(vertices, numFloats)
    , m_ownedShader(std::move(shader))
{}

void DrawableObject::update(float deltat){
    // TO be implemented
}

void DrawableObject::setTransform(std::shared_ptr<Transform> t){
    m_transform = std::move(t);
}

void DrawableObject::draw(ShaderProgram& fallbackShader){
    ShaderProgram& shader = m_ownedShader ? *m_ownedShader : fallbackShader;
    shader.use();
    glm::mat4 Matrice = m_transform ? m_transform->getMatrix() : glm::mat4(1.f); // If transform is nullptr, use identity matrix 
    shader.setMat4("modelMatrix", Matrice); 
     
    m_model.draw();
}
