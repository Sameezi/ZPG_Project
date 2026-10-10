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
#include "SceneNode.h"

class DrawableObject : public SceneNode{
    public:
    // uses its own shader, built from source
        DrawableObject(const std::vector<float>& vertices, std::unique_ptr<ShaderProgram> shader);
        DrawableObject(const std::vector<float>& vertices, int numFloats, std::unique_ptr<ShaderProgram> shader);
    // no shader — scene will supply one at draw time
        explicit DrawableObject(const std::vector<float>& vertices);

        void draw(const glm::mat4& parentWorld,ShaderProgram& fallbackShader) override;
        void update(float deltat);

        DrawableObject(const DrawableObject&) = delete;
        DrawableObject& operator=(const DrawableObject&) = delete;
        DrawableObject(DrawableObject&&) = default;
        DrawableObject& operator=(DrawableObject&&) = default;

    private:
       Model m_model; 
       std::unique_ptr<ShaderProgram> m_ownedShader;
       glm::vec3 position{0.0f}; 
};