#pragma once
#include "DrawableObject.h"

class ShaderProgram;
class Light : public DrawableObject{
    public:
       void addObserver(Observer<Light>* obs);
       void delObserver(Observer<Light>* obs);
       void notify();
        Light(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> emissiveShader, 
              glm::vec3 color = glm::vec3(1.f), float intensity = 1.f);
        void setLightPos(); 
    private:
        std::vector<Observer<Light>*> observers;
        glm::vec3 m_color;
        float m_intensity;

};