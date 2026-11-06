/**
 * @file Skybox.cpp
 *
 * @brief Skybox class implementation
 *
 * @author KUC0438 
  **/

#include "Skybox.h"
#include <stdexcept>

bool Skybox::loadOBJ(const std::string& path, std::vector<glm::vec3>& out)
{
    std::ifstream file(path);
    if (!file) return false;

    std::vector<glm::vec3> positions;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            glm::vec3 p;
            ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        }
        else if (type == "f") {
            std::vector<glm::vec3> face;
            std::string vert;
            while (ss >> vert) {
                int pi = 0;
                if (sscanf(vert.c_str(), "%d", &pi) != 1) return false;

                // záporné indexy v OBJ = odzadu
                int idx = (pi > 0) ? pi - 1 : (int)positions.size() + pi;
                if (idx < 0 || idx >= (int)positions.size()) return false;

                face.push_back(positions[idx]);
            }

            for (size_t i = 1; i + 1 < face.size(); ++i) {
                out.push_back(face[0]);
                out.push_back(face[i]);
                out.push_back(face[i + 1]);
            }
        }
    }
    return true;
}

Skybox::Skybox(const std::string& modPath, std::unique_ptr<Cubemap> c, std::unique_ptr<ShaderProgram> sh )
{
    std::vector<glm::vec3> vertices;
    loadOBJ(modPath, vertices);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);   // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                        (void*)offsetof(Vertex, position));

    glBindVertexArray(0);
    vertexCount = (GLsizei)vertices.size();

    m_cubemap = std::move(c);
    m_shader = std::move(sh);
}


Skybox::~Skybox() {
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
}

void Skybox::draw() const {
    glDepthMask(GL_FALSE);
    m_shader->use();

    glBindVertexArray(vao);
    if (m_cubemap) m_cubemap->bind(0);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glDepthMask(GL_TRUE);
    glBindVertexArray(0);
}

void Skybox::setTransform(std::unique_ptr<Transform> t){
    m_transform = std::move(t);
}


