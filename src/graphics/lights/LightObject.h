#pragma once
#include "../DrawableObject.h"
#include "Light.h"


class LightObject : public DrawableObject{
    public:
         LightObject(Model& model, ShaderProgram& emissiveShader,unsigned int id,std::shared_ptr<Light>);
         void pack(LightsBlock& b); 
         void pack(LightsBlock& b, Camera& c); 
         void enable();
    private:
        std::shared_ptr<Light> m_light;
};
