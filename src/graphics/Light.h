#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "DrawableObject.h"

class Model;
class ShaderProgram;
template <typename T> class Observer;

class Light : public DrawableObject {
public:
    Light(Model& model, ShaderProgram& emissiveShader,
          glm::vec3 color = glm::vec3(1.0f), float intensity = 1.0f, unsigned int id = -1);

    void addObserver(Observer<Light>* obs);
    void delObserver(Observer<Light>* obs);
    void notify();
    void setLightPos();

    glm::vec3 getColor() const;
    float getIntensity() const;

private:
    std::vector<Observer<Light>*> observers;  
    glm::vec3 m_color;
    float     m_intensity;
};