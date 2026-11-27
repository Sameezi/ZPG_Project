#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include "Observer.h"

class Camera{
    public:
       void addObserver(Observer<Camera>* obs);
       void delObserver(Observer<Camera>* obs);
       void notify();

       glm::mat4 getViewMatrix() const;
       glm::mat4 getProjMatrix() const;

       void setProjMatrix(glm::mat4 proj);
       glm::vec3 getFront() const;
       glm::vec3 getPos() const;

       void rotate(float deltaYaw, float deltaPitch);
       void setMovement(glm::vec3 offset);

       void updateView();
       void update(float deltaTime);

       void setAspectRatio(float aspect);
    private:
        glm::mat4 viewMatrix{1.0f};
        glm::mat4 projectionMatrix{1.0f};
        std::vector<Observer<Camera>*> observers;

        float yaw{-90.0f};
        float pitch{0.0f};
        float fov{60.0f};
        float nearPlane{0.1f};
        float farPlane{160.0f};

        glm::vec3 movement{0.0f, 0.0f, 0.0f};
        glm::vec3 position{0.0f, 2.0f, 4.0f};

};