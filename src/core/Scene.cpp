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
    m_defaultShader = std::make_unique<ShaderProgram>("shaders/lighting_texture.vert", "shaders/phong_texture_multi.frag");
    m_defaultShader->setCamera(getCamera());

    glGenBuffers(1, &lightsUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightsBlock), nullptr, GL_DYNAMIC_DRAW);  
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightsUBO);                              
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Scene::update(float deltat) {
    for (auto& [name, obj] : m_objects) {
        obj->update(deltat);
    }
    for (auto& [name, obj] : m_lightObjects) {
        obj->update(deltat);
    }
}

DrawableObject& Scene::addObject(const std::string& name, Model& model,ShaderProgram& shader){
    int id = getUniqID();
    if(id < 0){
        throw std::runtime_error("Couldn't add new object, limit of unique ids reached!");
    }
    auto obj = std::make_unique<DrawableObject>(model,shader,id);
    // save into id map
    DrawableObject* raw = obj.get();
    m_objectsById[raw->getId()] = raw;

    auto [it, inserted] = m_objects.try_emplace(name, std::move(obj));
    shader.setCamera(getCamera());
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
    shader.setCamera(getCamera());
    return *it->second;
}

LightObject& Scene::addLightObject(const std::string& name, Model& model, ShaderProgram& shader, std::shared_ptr<Light> light ){
    unsigned int id = getUniqID();
    if(id < 0){
        throw std::runtime_error("Couldn't add new object, limit of unique ids reached!");
    }
    auto obj = std::make_unique<LightObject>(model,shader,id,std::move(light));
    // save into id map
    DrawableObject* raw = obj.get();
    m_objectsById[raw->getId()] = raw;

    auto [it, inserted] = m_lightObjects.try_emplace(name, std::move(obj));
    shader.setCamera(getCamera());
    return *it->second;
}


void Scene::draw() {
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if(m_skybox) m_skybox->draw();

    LightsBlock block{};
    for (auto& [name, obj] : m_lightObjects) {
        obj->pack(block, *getCamera());
    }

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightsUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightsBlock), &block);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    for (auto& [name, obj] : m_lightObjects) {
        glStencilFunc(GL_ALWAYS, obj->getId(), 0xFF);
        obj->draw(*m_defaultShader);
    }

    for (auto& [name, obj] : m_objects) {
        glStencilFunc(GL_ALWAYS, obj->getId(), 0xFF);
        obj->draw(*m_defaultShader);   // obj is unique_ptr
    }

    if(editorObject) editorObject->draw(*m_defaultShader);
   
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

LightObject& Scene::getLightObject(const std::string& name){
    return *m_lightObjects.at(name);
}

Skybox& Scene::addSkybox(const std::string& modelPath,Cubemap& model, ShaderProgram& shader){
    m_skybox = std::make_shared<Skybox>(modelPath, model, shader);
    shader.setCamera(getCamera());
    return *m_skybox;
}

unsigned int Scene::getUniqID(){
    if(nextId > 255){
        std::cerr << "Max id's reached!\n";
        return -1;
    }
    return nextId++;
}

void Scene::setEditorObjModel(Model& model) {
    editorAttr.model = &model;
    editorObject = std::make_unique<DrawableObject>(model);
    editorObject->setTransform(getEditorObjTransform());
}

void Scene::setEditorObjRotation(const glm::vec4& rotation) {
    editorAttr.rotation = rotation;
    if (editorObject)
        editorObject->setTransform(getEditorObjTransform());
}

void Scene::setEditorObjTranslation(const glm::vec3& translation) {
    editorAttr.translation = translation;
    if (editorObject)
        editorObject->setTransform(getEditorObjTransform());
}

Model& Scene::getEditorObjModel() {
    return *editorAttr.model;  
}

std::shared_ptr<TransformComposite> Scene::getEditorObjTransform() const {
    const glm::vec4& r = editorAttr.rotation;
    auto composite = std::make_shared<TransformComposite>();
    composite->addNode(std::make_shared<Translation>(editorAttr.translation));
    composite->addNode(std::make_shared<Rotation>(r.x, glm::vec3(r.y, r.z, r.w)));
    return composite;
}