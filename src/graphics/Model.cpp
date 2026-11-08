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

bool Model::loadOBJ(const std::string& path, std::vector<Vertex>& out)
{
    std::ifstream file(path);
    if (!file) return false;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

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
        else if (type == "vt") {
            glm::vec2 t;
            ss >> t.x >> t.y;
            uvs.push_back(t);
        }
        else if (type == "vn") {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }
        else if (type == "f") {
            std::string vert;
            std::vector<Vertex> face;

            while (ss >> vert) {
                // formát p/t/n
                int pi = 0, ti = 0, ni = 0;
                if (sscanf(vert.c_str(), "%d/%d/%d", &pi, &ti, &ni) != 3)
                    return false;   

                Vertex v;
                v.position = positions[pi - 1];  
                v.uv       = uvs[ti - 1];
                v.normal   = normals[ni - 1];
                face.push_back(v);
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

        
Model::Model(const std::string& modPath, const std::string& texPath)
{
    std::vector<Vertex> vertices;
    if(!loadOBJ(modPath, vertices)){
        std::string message = "Couldn't find "+modPath+" , file is not .obj or does not exist!";
        throw std::invalid_argument(message);
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);   // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);   // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);   // normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);
    vertexCount = (GLsizei)vertices.size();

    std::cout<<modPath<<" loaded with " << vertexCount << " vertices"<<std::endl;
    addTexture(0,std::make_unique<Texture>(texPath));
}

Model::Model(const std::vector<float>& vertices, const std::string& texPath)
{
    if (vertices.empty() || vertices.size() % 8 != 0) {
        throw std::invalid_argument("Model vertices must contain complete position+normal+uv data");
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    vertexCount = (GLsizei)vertices.size();

    addTexture(0, std::make_shared<Texture>(texPath));
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
    for (auto& slot: m_textures){
        slot.texture->bind(slot.unit);
    }
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0); // Default state
    ++RenderStats::drawCalls;
}

void Model::addTexture(GLuint unit, std::shared_ptr<Texture> tex) {
    for (auto& slot : m_textures) {
        if (slot.unit == unit) { slot.texture = std::move(tex); return; }
    }
    m_textures.push_back({unit, std::move(tex)});
}

void Model::addTexture(GLuint unit, const std::string& path) {
    addTexture(unit, std::make_shared<Texture>(path));
}

void Model::removeTexture(GLuint unit) {
    std::erase_if(m_textures, [unit](const TextureSlot& s){ return s.unit == unit; });
}

int Model::getTextureCount(){
    return m_textures.size();
}



