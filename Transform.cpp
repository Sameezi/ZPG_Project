#include "Transform.h"

glm::mat4 Transform::getMatrix() const{
    glm::mat4 m(1.0f); 
    m = glm::translate(glm::mat4(1.0f), position);
    m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1,0,0));
    m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0,1,0));
    m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0,0,1));
    m = glm::scale(m,scale);
    return m;
}

void Transform::setPosition(glm::vec3 position){
    this->position = position;
}

void Transform::setRotation(glm::vec3 rotation){
    this->rotation = rotation;
}

void Transform::setScale(glm::vec3 scale){
    this->scale = scale;
}

void Transform::setScale(float uniform){
    setScale(glm::vec3(uniform));
}
