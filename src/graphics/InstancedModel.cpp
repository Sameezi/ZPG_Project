/**
 * @file InstancedModel.cpp
 *
 * @brief InstancedModel Implementation
 *
 * @author KUC0438 
  **/
#include "InstancedModel.h"

InstancedModel::InstancedModel(const std::vector<float>& vertices) 
    : Model(vertices)
{
    glBindVertexArray(vao);

    glGenBuffers(1, &ivbo);
    glBindBuffer(GL_ARRAY_BUFFER, ivbo);

    for (int i = 0; i < 4; ++i) {
        glEnableVertexAttribArray(2 + i);
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE,
                              sizeof(glm::mat4),
                              (void*)(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(2 + i, 1);
    }

    glBindVertexArray(0);
}

InstancedModel::~InstancedModel() {
    if (ivbo != 0) {
        glDeleteBuffers(1, &ivbo);
    }
}

void InstancedModel::setInstances(const std::vector<TransformComposite>& composites){
    std::vector<glm::mat4> matrices;
    for (auto& composite : composites){
        matrices.push_back(composite.getMatrix());
    }
    icount = static_cast<GLsizei>(matrices.size());
    glBindBuffer(GL_ARRAY_BUFFER, ivbo);
    glBufferData(GL_ARRAY_BUFFER,
                 matrices.size() * sizeof(glm::mat4),
                 matrices.data(), GL_DYNAMIC_DRAW);
}

void InstancedModel::draw() const{
    if(icount == 0) return;
    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, icount);
    glBindVertexArray(0);
    ++RenderStats::drawCalls;
    RenderStats::instances += icount;
}
