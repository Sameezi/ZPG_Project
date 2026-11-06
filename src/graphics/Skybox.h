#pragma once

#include <glad/gl.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <memory>
#include "Cubemap.h"
#include "ShaderProgram.h"
#include "Vertex.h"
#include "../transformation/Transform.h"

class Skybox{
    public:
        Skybox(const std::string& modPath, std::unique_ptr<Cubemap> c, std::unique_ptr<ShaderProgram> sh );
        ~Skybox();

        void draw() const; 
        void setTransform(std::unique_ptr<Transform> t);
    private:
        std::vector<float> m_vertices;
        GLuint vbo{0};
        GLuint vao{0};
        GLsizei vertexCount{0};
        std::unique_ptr<ShaderProgram> m_shader;
        std::unique_ptr<Cubemap> m_cubemap;
        std::unique_ptr<Transform> m_transform;
        bool loadOBJ(const std::string& path, std::vector<glm::vec3>& out);
};