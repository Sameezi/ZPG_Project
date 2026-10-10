#pragma once
#include <glad/gl.h>
#include <vector>
#include "SceneNode.h"
#include <memory>
#include "ShaderProgram.h"

class GroupNode : public SceneNode{
    public:
        SceneNode& addNode(std::unique_ptr<SceneNode> child);
        void draw(const glm::mat4& parentWorld, ShaderProgram& fallbackShader) override;

    private:
        std::vector<std::unique_ptr<SceneNode>> children;
};