#pragma once
#include "Transform.h"


class Translation : public Transform{
    public:
        explicit Translation(glm::vec3 offset) : offset(offset) {}
        glm::mat4 getMatrix() const override;
    private:
        glm::vec3 offset;
};