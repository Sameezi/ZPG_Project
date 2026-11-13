#pragma once
#include <glad/gl.h>

//Include the standard C++ headers  
#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "graphics/DrawableObject.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Model.h"
#include "graphics/Light.h"
#include "Camera.h"
#include "graphics/Skybox.h"
#include "transformation/Translation.h"
#include "transformation/Rotation.h"


class Scene{
    public:
        Scene();
        ~Scene();
        void init();
        void update();
        void draw();
        DrawableObject& addObject(const std::string& name, Model& model, ShaderProgram& shader);
        DrawableObject& addObject(Model& model, ShaderProgram& shader);
        DrawableObject& getObject(const std::string& name);
        DrawableObject& getObject(GLuint id = -1);
        bool removeObject(GLuint id = -1);

        Light& addLightObject(const std::string& name, Model& model, ShaderProgram& shader, 
                         glm::vec3 color = glm::vec3(1.0f), float intensity = 1.0f);
        Light& getLightObject(const std::string& name);

        Skybox& addSkybox(std::shared_ptr<Skybox> sky);
                  
        void setEditorObjModel(Model& model);
        void setEditorObjRotation(const glm::vec4& rotation);      
        void setEditorObjTranslation(const glm::vec3& translation);
        Model& getEditorObjModel();
        std::shared_ptr<TransformComposite> getEditorObjTransform() const;

        glm::vec4& getBackgroundColor();
        Camera* getCamera();
    private:
        std::unordered_map<std::string, std::unique_ptr<DrawableObject>> m_objects; 
        std::unordered_map<GLuint, DrawableObject*> m_objectsById;
        std::unordered_map<std::string, std::unique_ptr<Light>> m_lightObjects;
        std::unique_ptr<ShaderProgram> m_defaultShader;
        std::shared_ptr<Skybox> m_skybox;

        struct EditorAttr {
            Model* model = nullptr;
            glm::vec3 translation{0.f};
            glm::vec4 rotation{0.f, 0.f, 1.f, 0.f};
        } editorAttr;
        std::unique_ptr<DrawableObject> editorObject;

        glm::vec4 backgroundColor{0.0f,0.0f,0.0f,0.0f};
        Camera m_camera;
        unsigned int nextId = 1;
        unsigned int getUniqID();
};
