/**
 * @file Translation.cpp
 *
 * @brief Translation Implementation
 *
 * @author KUC0438 
  **/

#include "Translation.h"


glm::mat4 Translation::getMatrix() const{
    return glm::translate(glm::mat4(1.0f), offset);
}