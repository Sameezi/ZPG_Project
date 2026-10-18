#pragma once

#include <glad/gl.h>
#include "RenderStats.h"
#include <vector>

class Model{
    public:
        Model(const std::vector<float>& vertices);
        Model(const std::vector<float>& vertices, int numFloats);
        virtual ~Model();

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        virtual void draw() const; 
    private:
        std::vector<float> m_vertices;
        GLuint vbo{0};
    protected:
        GLsizei vertexCount{0};
        GLuint vao{0};
};