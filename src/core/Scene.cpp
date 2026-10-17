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
    m_defaultShader = std::make_unique<ShaderProgram>("shaders/projective.vert", "shaders/basic.frag",&m_camera);
}

void Scene::update() {
    // camera/animation updates to be included..
}

DrawableObject& Scene::addObject(const std::string& name, std::unique_ptr<DrawableObject> obj){
    auto [it, inserted] = m_objects.try_emplace(name, std::move(obj));
    return *it->second;
}

void Scene::draw() {
    for (auto& [name, obj] : m_objects) {
    obj->draw(*m_defaultShader);   // obj is unique_ptr
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