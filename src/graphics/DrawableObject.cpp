/**
 * @file DrawableObject.cpp
 *
 * @brief DrawableProject Implementation
 *
 * @author KUC0438 
  **/
#include "DrawableObject.h"
#include <random>

DrawableObject::DrawableObject(Model& model, ShaderProgram& shader, unsigned int id)
    : m_model(&model)
    , m_shader(&shader)
    , m_id(id)
{}

DrawableObject::DrawableObject(Model& model, unsigned int id)
    : m_model(&model)
    , m_shader(nullptr)
    , m_id(id)
{}


static glm::vec3 randomPoint(const glm::vec3& mn, const glm::vec3& mx) {
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dx(mn.x, mx.x), dy(mn.y, mx.y), dz(mn.z, mx.z);
    return { dx(rng), dy(rng), dz(rng) };
}

void DrawableObject::update(float deltat) {
    if(m_hasAxis){
        float len = glm::length(m_axis.B - m_axis.A);
        m_axis.t +=  (len > 1e-4f) ? (m_axis.speed * deltat / len) : 1.0f; 

        if (m_axis.t >= 1.0f) { 
            setPos(m_axis.B);
            m_axis.A = m_axis.B;
            m_axis.B = randomPoint(m_axis.boundsMin, m_axis.boundsMax);
            m_axis.t = 0.0f;
        } else {
            setPos(glm::mix(m_axis.A, m_axis.B, m_axis.t));
        }
    }else{
        positionOffset += acceleration * deltat;
    }
}

void DrawableObject::startRoaming(glm::vec3 boundMin, glm::vec3 boundMax, float speed){
    m_axis.boundsMin = boundMin;
    m_axis.boundsMax = boundMax;
    setParametricAxis(randomPoint(boundMin, boundMax));
    setParametricAxis(randomPoint(boundMin, boundMax));
    m_axis.speed = speed;
}


void DrawableObject::setTransform(std::shared_ptr<Transform> t) {
    m_transform = std::move(t);
    positionOffset = glm::vec3(0.0f);
    m_axis.A = getPos();
}

void DrawableObject::setParametricAxis(glm::vec3 pos){
	if(m_axis.index == 0){
		std::cout << "A: " << pos.x << " " << pos.y << " " << pos.z << std::endl; 
		m_axis.A = pos;
		m_axis.index = 1;
        m_hasAxis = false;
	}else{
		std::cout << "B: " << pos.x << " " << pos.y << " " << pos.z << std::endl; 
		m_axis.B = pos;
		m_axis.index = 0;
		setPos(m_axis.A);
        m_hasAxis = true;
        m_axis.t = 0.0f;
	}
}

void DrawableObject::setTime(float time) {
    if (!m_shader) return;   
    m_shader->use();
    m_shader->setTime(time);
}

void DrawableObject::draw(ShaderProgram& fallbackShader) {
    ShaderProgram& shader = m_shader ? *m_shader : fallbackShader;
    shader.use();

    glm::mat4 matrice = m_transform ? m_transform->getMatrix() : glm::mat4(1.f);
    matrice[3] += glm::vec4(positionOffset, 0.f);

    if(m_material){
        shader.setMaterial(*m_material);
    }

    if (!dynamic_cast<InstancedModel*>(m_model))
        shader.setMat4("modelMatrix", matrice);
    if(m_model->getTextureCount() > 1)
        for(int i = 0; i < m_model->getTextureCount(); i++)
            shader.setInt("textureUnit" + std::to_string(i+1), i);

    m_model->draw();
}

glm::vec3 DrawableObject::getPos() const {
    glm::mat4 m = m_transform ? m_transform->getMatrix() : glm::mat4(1.f);
    return glm::vec3(m[3]) + positionOffset;
}

void DrawableObject::setPos(glm::vec3 pos){
    glm::mat4 transform = m_transform ? m_transform->getMatrix() : glm::mat4(1.f);
    positionOffset = pos - glm::vec3(transform[3]);
}

void DrawableObject::setAcc(glm::vec3 offset) {
    acceleration = offset;
}

unsigned int DrawableObject::getId() const{
    return m_id;
}

void DrawableObject::setMaterial(Material material){
    m_material = std::move(material);
}
