/**
 * @file Scaling.cpp
 *
 * @brief Scaling Implementation
 *
 * @author KUC0438 
  **/
#include "Scaling.h"

glm::mat4 Scaling::getMatrix() const {
    return glm::scale(glm::mat4(1.f),factors);
}