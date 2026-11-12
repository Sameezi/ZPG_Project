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

DrawableObject& Scene::addObject(const std::string& name, Model& model,ShaderProgram& shader){
    unsigned int id = getUniqID();
    if(id < 0){
        throw std::runtime_error("Couldn't add new object, limit of unique ids reached!");
    }
    auto obj = std::make_unique<DrawableObject>(model,shader,id);
    // save into id map
    DrawableObject* raw = obj.get();
    m_objectsById[raw->getId()] = raw;

    auto [it, inserted] = m_objects.try_emplace(name, std::move(obj));
    return *it->second;
}

DrawableObject& Scene::addObject(Model& model,ShaderProgram& shader){
    unsigned int id = getUniqID();
    if(id < 0){
        throw std::runtime_error("Couldn't add new object, limit of unique ids reached!");
    }
    auto obj = std::make_unique<DrawableObject>(model,shader,id);
    // save into id map
    DrawableObject* raw = obj.get();
    m_objectsById[raw->getId()] = raw;

    const std::string& name = "obj-" + std::to_string(id);
    auto [it, inserted] = m_objects.try_emplace(name, std::move(obj));
    return *it->second;
}

Light& Scene::addLightObject(const std::string& name, Model& model, ShaderProgram& shader, glm::vec3 color, float intensity){
    unsigned int id = getUniqID();
    if(id < 0){
        throw std::runtime_error("Couldn't add new object, limit of unique ids reached!");
    }
    auto obj = std::make_unique<Light>(model,shader,color,intensity,id);
    // save into id map
    DrawableObject* raw = obj.get();
    m_objectsById[raw->getId()] = raw;

    auto [it, inserted] = m_lightObjects.try_emplace(name, std::move(obj));
    return *it->second;
}

void Scene::draw() {

    if(m_skybox) m_skybox->draw();

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    for (auto& [name, obj] : m_objects) {
        glStencilFunc(GL_ALWAYS, obj->getId(), 0xFF);
        obj->draw(*m_defaultShader);   // obj is unique_ptr
    }

    for(auto& [name,obj] : m_lightObjects){
        glStencilFunc(GL_ALWAYS, obj->getId(), 0xFF);
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


DrawableObject& Scene::getObject(GLuint id){
    if(id == -1){
        return *m_objectsById.at(nextId-1);
    }
    return *m_objectsById.at(id);
}

bool Scene::removeObject(GLuint id){ // Only user objects can be deleted!
    const std::string name = "obj-" + std::to_string(id);
    auto it = m_objects.find(name);
    if (it == m_objects.end()){
        std::cerr << "Object not found!\n";
        return false;
    }
    m_objectsById.erase(it->second->getId());  // first index, then owner
    m_objects.erase(it);
    return true;
}

Light& Scene::getLightObject(const std::string& name){
    return *m_lightObjects.at(name);
}

Skybox& Scene::addSkybox(std::shared_ptr<Skybox> sky){
    m_skybox = std::move(sky);
    return *m_skybox;
}

unsigned int Scene::getUniqID(){
    if(nextId > 255){
        std::cerr << "Max id's reached!\n";
        return -1;
    }
    return nextId++;
}
