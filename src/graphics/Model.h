#pragma once

#include <glad/gl.h>

#include <vector>

class Model{
    public:
        Model(const std::vector<float>& vertices);
        Model(const std::vector<float>& vertices, int numFloats);
        ~Model();

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        void draw() const; 
    private:
        std::vector<float> m_vertices;
        GLuint vao{0}, vbo{0};
        GLsizei vertexCount{0};

};