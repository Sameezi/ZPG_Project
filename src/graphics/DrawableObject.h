#pragma once
#include <glad/gl.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include the standard C++ headers  
#include <vector>
#include <string>
#include <memory>

#include "Model.h"
#include "InstancedModel.h"
#include "ShaderProgram.h"
#include "transformation/Transform.h"

class ShaderProgram;
class DrawableObject{
    public:
    // uses its own shader, built from source
        DrawableObject(std::shared_ptr<Model> model, std::shared_ptr<ShaderProgram> shader);
    // no shader — scene will supply one at draw time
        explicit DrawableObject(std::shared_ptr<Model> model);
        
        void draw(ShaderProgram& fallbackShader);
        void update(float deltat);
        void setTransform(std::shared_ptr<Transform> t);
        void setTime(float time);

        glm::vec3 getPos() const;
        void setPos(glm::vec3 pos) const;

        DrawableObject(const DrawableObject&) = delete;
        DrawableObject& operator=(const DrawableObject&) = delete;
        DrawableObject(DrawableObject&&) = default;
        DrawableObject& operator=(DrawableObject&&) = default;

    private:
       std::shared_ptr<Model> m_model; 
       std::shared_ptr<Transform> m_transform;
    protected:
        std::shared_ptr<ShaderProgram> m_shader;
};