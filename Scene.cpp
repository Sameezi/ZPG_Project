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
    root = std::make_unique<GroupNode>();
}

void Scene::update() {
    // camera/animation updates to be included..
}

DrawableObject& Scene::addObject(const std::string& name, std::unique_ptr<DrawableObject> obj, const std::string& parent) {
    GroupNode* target = parent.empty() ? root.get() : m_groups.at(parent);
    DrawableObject& ref = *obj;
    m_objects[name] = &ref;

    target->addNode(std::move(obj));
    return ref;
}

GroupNode& Scene::addGroup(const std::string& name,const std::string& parent){ 
    GroupNode* target = parent.empty() ? root.get() : m_groups.at(parent);
    auto group = std::make_unique<GroupNode>();
    GroupNode& ref = *group;
    m_groups[name] = &ref;

    target->addNode(std::move(group));
    return ref;
}

void Scene::draw() {
    root->draw(glm::mat4(1.0f), *m_defaultShader);
}


glm::vec4& Scene::getBackgroundColor(){
    return backgroundColor;
}

DrawableObject& Scene::getObject(const std::string& name){
    return *m_objects.at(name);
}

GroupNode& Scene::getGroup(const std::string& name){
    return *m_groups.at(name);
}