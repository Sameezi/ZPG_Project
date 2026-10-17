#pragma once
#include <glad/gl.h>

//Include the standard C++ headers  
#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "graphics/DrawableObject.h"
#include "graphics/ShaderProgram.h"
#include "Camera.h"
class Scene{
    public:
        Scene();
        ~Scene();
        void init();
        void update();
        void draw();
        DrawableObject& addObject(const std::string& name, std::unique_ptr<DrawableObject> obj);
        DrawableObject& getObject(const std::string& name);

        glm::vec4& getBackgroundColor();
        Camera* getCamera();
    private:
        std::unordered_map<std::string, std::unique_ptr<DrawableObject>> m_objects; 
        std::unique_ptr<ShaderProgram> m_defaultShader;
        glm::vec4 backgroundColor{0.0f,0.0f,0.0f,0.0f};
        Camera m_camera;
};
