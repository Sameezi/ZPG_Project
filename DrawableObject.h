#pragma once
#include <glad/gl.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include the standard C++ headers  
#include <vector>
#include <string>
#include <memory>

#include "Model.h"
#include "ShaderProgram.h"
#include "Transform.h"

class DrawableObject{
    public:
    // uses its own shader, built from source
        DrawableObject(const std::vector<float>& vertices, std::unique_ptr<ShaderProgram> shader);
        DrawableObject(const std::vector<float>& vertices, int numFloats, std::unique_ptr<ShaderProgram> shader);
    // no shader — scene will supply one at draw time
        explicit DrawableObject(const std::vector<float>& vertices);

        void draw(ShaderProgram& fallbackShader);
        void update(float deltat);
        void setTransform(std::shared_ptr<Transform> t);


        DrawableObject(const DrawableObject&) = delete;
        DrawableObject& operator=(const DrawableObject&) = delete;
        DrawableObject(DrawableObject&&) = default;
        DrawableObject& operator=(DrawableObject&&) = default;

    private:
       Model m_model; 
       std::unique_ptr<ShaderProgram> m_ownedShader;
       std::shared_ptr<Transform> m_transform;
       glm::vec3 position{0.0f}; 
};