/**
 * @file Rotation.cpp
 *
 * @brief Rotation Implementation
 *
 * @author KUC0438 
  **/
#include "Rotation.h"


glm::mat4 Rotation::getMatrix() const {
    float current = angle + speed * static_cast<float>(glfwGetTime());
    return glm::rotate(glm::mat4 (1.f), current ,axis);
}
