/**
 * @file Model.cpp
 *
 * @brief Model class implementation
 *
 * @author KUC0438 
  **/

#include "Model.h"

#include <stdexcept>

Model::Model(const std::vector<float>& vertices)
    : vertexCount(static_cast<GLsizei>(vertices.size() / 6)) // pos+normal = 6 floats/vertex
{
    if (vertices.empty() || vertices.size() % 6 != 0) {
        throw std::invalid_argument("Model vertices must contain complete position+normal data");
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}



Model::Model(const std::vector<float>& vertices, int numFloats)
    : vertexCount(static_cast<GLsizei>(vertices.size() / numFloats)) // numFloats floats/vertex
{
    if (numFloats <= 0 || vertices.empty() ||
        vertices.size() % static_cast<size_t>(numFloats) != 0) {
        throw std::invalid_argument(
            "Model vertices must contain complete vertices of " +
            std::to_string(numFloats) + " floats each");
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, numFloats * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    for(int i = 1; i < numFloats / 3; i++){
    glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, numFloats * sizeof(float), (void*)((3 * i) * sizeof(float)));
    glEnableVertexAttribArray(i);
    }
    
    glBindVertexArray(0);
}

Model::~Model() {
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
}

void Model::draw() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
    ++RenderStats::drawCalls;
}


