/**
 * @file GroupNode.cpp
 *
 * @brief GroupNode implementation 
 *
 * @author KUC0438 
  **/

#include "GroupNode.h"

SceneNode& GroupNode::addNode(std::unique_ptr<SceneNode> child){
    children.push_back(std::move(child));
    return *children.back();
}
void GroupNode::draw(const glm::mat4& parentWorld, ShaderProgram& fallbackShader){
    glm::mat4 world = parentWorld * m_transform.getMatrix();
    for (auto& child : children){
        child->draw(world, fallbackShader);
    }
}