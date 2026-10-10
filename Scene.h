#pragma once
#include <glad/gl.h>

//Include the standard C++ headers  
#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "DrawableObject.h"
#include "ShaderProgram.h"
#include "GroupNode.h"
class Scene{
    public:
        Scene();
        ~Scene();
        void init();
        void update();
        void draw();
        DrawableObject& addObject(const std::string& name, std::unique_ptr<DrawableObject> obj, const std::string& parent = "");
        GroupNode& addGroup(const std::string& name, const std::string& parent = "");
        
        DrawableObject& getObject(const std::string& name);
        GroupNode& getGroup(const std::string& name);

        glm::vec4& getBackgroundColor();
    private:
        std::unordered_map<std::string, DrawableObject*> m_objects;
        std::unordered_map<std::string, GroupNode*> m_groups;
        std::unique_ptr<ShaderProgram> m_defaultShader;
        std::unique_ptr<GroupNode> root;
        glm::vec4 backgroundColor{0.0f,0.0f,0.0f,0.0f};
};
