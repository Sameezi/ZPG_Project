/**
 * @file Scene.cpp
 *
 * @brief Scene class implementation
 *
 * @author KUC0438 
  **/


#include "Scene.h"

Scene::Scene() = default;
Scene::~Scene() = default;

void Scene::init() {
    m_defaultShader = std::make_unique<ShaderProgram>("shaders/basic.vert", "shaders/basic.frag");
}

void Scene::update() {
    // camera/animation updates to be included..
}

DrawableObject& Scene::addObject(const std::string& name, std::unique_ptr<DrawableObject> obj){
    auto [it, inserted] = m_objects.try_emplace(name, std::move(obj));
    return *it->second;
}

Light& Scene::addObject(const std::string& name, std::unique_ptr<Light> obj){
    auto [it, inserted] = m_lightObjects.try_emplace(name, std::move(obj));
    return *it->second;
}

void Scene::draw() {

    if(m_skybox) m_skybox->draw();

    for (auto& [name, obj] : m_objects) {
    obj->draw(*m_defaultShader);   // obj is unique_ptr
    }
    for(auto& [name,obj] : m_lightObjects){
        obj->draw(*m_defaultShader);
        obj->notify();
    }
   
}

Camera* Scene::getCamera(){
    return &m_camera;
}

glm::vec4& Scene::getBackgroundColor(){
    return backgroundColor;
}

DrawableObject& Scene::getObject(const std::string& name){
    return *m_objects.at(name);
}

Light& Scene::getLightObject(const std::string& name){
    return *m_lightObjects.at(name);
}

Skybox& Scene::addSkybox(std::shared_ptr<Skybox> sky){
    m_skybox = std::move(sky);
    return *m_skybox;
}