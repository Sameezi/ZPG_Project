#include "Light.h"


Light::Light(std::shared_ptr<Model> model,
          std::shared_ptr<ShaderProgram> emissiveShader,
          glm::vec3 color, float intensity)
        : DrawableObject(std::move(model), std::move(emissiveShader)),
          m_color(color), m_intensity(intensity) {}


void Light::addObserver(Observer<Light>* obs){
    observers.push_back(obs);
    obs->update(*this);
}

void Light::delObserver(Observer<Light>* obs){
    //Later
}

void Light::notify(){
    for (Observer<Light>* obs : observers){
        obs->update(*this);
    } 
}