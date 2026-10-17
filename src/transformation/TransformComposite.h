#pragma once
#include "Transform.h"
#include <vector>
#include <memory>

class TransformComposite : public Transform {
    public:
        void addNode(std::shared_ptr<Transform> transformation);
        glm::mat4 getMatrix() const override;
    private:
        std::vector<std::shared_ptr<Transform>> children;
};



