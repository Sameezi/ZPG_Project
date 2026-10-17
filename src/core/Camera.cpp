/**
 * @file Camera.cpp
 *
 * @brief Camera class implementation
 *
 * @author KUC0438 
  **/

#include "Camera.h"

void Camera::addObserver(Observer* obs){
    observers.push_back(obs);
    obs->update(*this);
}

void Camera::delObserver(Observer* obs){
    //Later
}

void Camera::notify(){
    for (Observer* obs : observers){
        obs->update(*this);
    } 
}

glm::mat4 Camera::getViewMatrix() const{
    return viewMatrix;
}

glm::mat4 Camera::getProjMatrix() const{
    return projectionMatrix;
}

void Camera::setProjMatrix(glm::mat4 proj){
    projectionMatrix = proj;
    notify();
}

glm::vec3 Camera::getFront(){
    return glm::normalize(glm::vec3{
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    });
}

void Camera::rotate(float deltaYaw, float deltaPitch){
    this->yaw += deltaYaw;
    pitch = std::clamp(pitch + deltaPitch, -89.0f, 89.0f);
}

void Camera::setMovement(glm::vec2 offset){
    this->movement = offset;
}

void Camera::updateView(){
    glm::vec3 up{
        0.0f,
        1.0f,
        0.0f
    };

    glm::vec3 front = getFront();
    viewMatrix = glm::lookAt(position, position + front, up);
    notify();
}

void Camera::update(float deltaTime){
    glm::vec3 up{
        0.0f,
        1.0f,
        0.0f
    };
    glm::vec3 front = getFront();
    glm::vec3 right = glm::normalize(glm::cross(front, up));

    position += right * movement.y * deltaTime; 
    position += front * movement.x * deltaTime;
    updateView();
}