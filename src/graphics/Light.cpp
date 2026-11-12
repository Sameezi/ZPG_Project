#include "Light.h"
#include <algorithm>

Light::Light(Model& model, ShaderProgram& emissiveShader,
             glm::vec3 color, float intensity, unsigned int id)
    : DrawableObject(model, emissiveShader,id)
    , m_color(color)
    , m_intensity(intensity)
{}

void Light::addObserver(Observer<Light>* obs) {
    observers.push_back(obs);
    obs->update(*this);
}

void Light::delObserver(Observer<Light>* obs) {
    observers.erase(std::remove(observers.begin(), observers.end(), obs),
                    observers.end());
}

void Light::notify() {
    for (Observer<Light>* obs : observers)
        obs->update(*this);
}

glm::vec3 Light::getColor() const{
    return m_color;
}

float Light::getIntensity() const{
    return m_intensity;
}