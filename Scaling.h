#pragma once
#include "Transform.h"

class Scaling : public Transform{
    public:
        explicit Scaling(glm::vec3 scale) : factors(factors) {}
        explicit Scaling(float uniform) :  factors(glm::vec3(uniform,uniform,uniform)) {}
        glm::mat4 getMatrix() const override;
    private:
        glm::vec3 factors;

};

