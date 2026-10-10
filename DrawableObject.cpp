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
    
}

void DrawableObject::draw(const glm::mat4& parentWorld,ShaderProgram& fallbackShader){
    ShaderProgram& shader = m_ownedShader ? *m_ownedShader : fallbackShader;
    shader.use();
    shader.setMat4("modelMatrix", parentWorld * m_transform.getMatrix()); 
     
    m_model.draw();
}
