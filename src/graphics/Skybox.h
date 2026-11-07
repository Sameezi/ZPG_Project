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
        Skybox(const std::string& modPath, Cubemap& cubemap, ShaderProgram& shader);
        ~Skybox();

        void draw() const; 
        void setTransform(std::shared_ptr<Transform> t);
    private:
        std::vector<float> m_vertices;
        GLuint vbo{0};
        GLuint vao{0};
        GLsizei vertexCount{0};

        ShaderProgram* m_shader;
        Cubemap* m_cubemap;
        std::shared_ptr<Transform> m_transform;
        static bool loadOBJ(const std::string& path, std::vector<glm::vec3>& out);
};