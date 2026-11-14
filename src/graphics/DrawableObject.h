#pragma once
#include <glad/gl.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include the standard C++ headers  
#include <vector>
#include <string>
#include <memory>

#include "Model.h"
#include "InstancedModel.h"
#include "ShaderProgram.h"
#include "transformation/Transform.h"
#include "core/parametricAxis.h"

class ShaderProgram;
class DrawableObject{
    public:
    // uses its own shader, built from source
        DrawableObject(Model& model, ShaderProgram& shader, unsigned int id = -1);
    // no shader — scene will supply one at draw time
        explicit DrawableObject(Model& model, unsigned int id = -1);
        
        void draw(ShaderProgram& fallbackShader);
        void update(float deltat);
        void setTransform(std::shared_ptr<Transform> t);
        void setTime(float time);
        void setAcc(glm::vec3 offset);
        void setParametricAxis(glm::vec3 pos);

        glm::vec3 getPos() const;
        void setPos(glm::vec3 pos);
        unsigned int getId() const;

    private:
       Model* m_model; 
       std::shared_ptr<Transform> m_transform;
       unsigned int m_id;
       glm::vec3 acceleration{0.0f};
       glm::vec3 position{0.0f};
       parametricAxis m_axis;
       bool m_hasAxis{false};
    protected:
        ShaderProgram* m_shader;
};