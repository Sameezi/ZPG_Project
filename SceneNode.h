#pragma once
#include <glad/gl.h>
#include "Transform.h"
#include "ShaderProgram.h"

class SceneNode{
    public:
        virtual ~SceneNode() = default;
        virtual void draw(const glm::mat4& parentWorld, ShaderProgram& fallbackShader) = 0;
        Transform& getTransform();
    protected:
        Transform m_transform;
};