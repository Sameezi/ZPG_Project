/**
 * @file TranslationComposite.cpp
 *
 * @brief TranslationComposite Implementation
 *
 * @author KUC0438 
  **/

#include "TransformComposite.h"


void TransformComposite::addNode(std::shared_ptr<Transform> transformation){
    children.push_back(std::move(transformation));
}

glm::mat4 TransformComposite::getMatrix() const {
    glm::mat4 m(1.f);
    for (auto& child : children) {
        m *= child->getMatrix();
    }
    return m;
}