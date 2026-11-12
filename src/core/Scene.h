#pragma once
#include <glad/gl.h>

//Include the standard C++ headers  
#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "graphics/DrawableObject.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Light.h"
#include "Camera.h"
#include "graphics/Skybox.h"

class Scene{
    public:
        Scene();
        ~Scene();
        void init();
        void update();
        void draw();
        DrawableObject& addObject(const std::string& name, std::unique_ptr<DrawableObject> obj);
        Light& addObject(const std::string& name, std::unique_ptr<Light> obj);
        DrawableObject& getObject(const std::string& name);
        Light& getLightObject(const std::string& name);
        Skybox& addSkybox(std::shared_ptr<Skybox> sky);
        
        unsigned int getUniqID();
        glm::vec4& getBackgroundColor();
        Camera* getCamera();
    private:
        std::unordered_map<std::string, std::unique_ptr<DrawableObject>> m_objects; 
        std::unordered_map<std::string, std::unique_ptr<Light>> m_lightObjects;
        std::unique_ptr<ShaderProgram> m_defaultShader;
        std::shared_ptr<Skybox> m_skybox;
        glm::vec4 backgroundColor{0.0f,0.0f,0.0f,0.0f};
        Camera m_camera;
        unsigned int nextId = 1;
        std::unordered_map<unsigned int, DrawableObject*> byId;
};
