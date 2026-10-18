#pragma once
#include "Model.h"
#include "transformation/TransformComposite.h"


class InstancedModel : public Model{
    public:
        explicit InstancedModel(const std::vector<float>& vertices);
        ~InstancedModel() override;
        void draw() const override;
        void setInstances(const std::vector<TransformComposite>& composites);
    private:
        GLuint ivbo{0};
        GLsizei icount{0};
};