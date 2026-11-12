#pragma once

#include <glad/gl.h>
#include "RenderStats.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <memory>
#include "Texture.h"
#include "Vertex.h"


class Model{
    public:
        Model(const std::vector<float>& vertices);
        Model(const std::vector<float>& vertices, int numFloats);
        Model(const std::string& modPath, const std::string& texPath);
        Model(const std::vector<float>& vertices, const std::string& texPath);

        virtual ~Model();

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        void addTexture(GLuint unit, std::shared_ptr<Texture> texture);
        void addTexture(GLuint unit, const std::string& path);
        void removeTexture(GLuint unit);
        int getTextureCount();

        virtual void draw() const; 
    private:
        struct TextureSlot {
            GLuint unit;
            std::shared_ptr<Texture> texture;
        };
        std::vector<TextureSlot> m_textures;
        std::vector<float> m_vertices;
        GLuint vbo{0};

        bool loadOBJ(const std::string& path, std::vector<Vertex>& out);
    protected:
        GLsizei vertexCount{0};
        GLuint vao{0};
};