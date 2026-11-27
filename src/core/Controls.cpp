#include "Controls.h"
#include <GLFW/glfw3.h>  

void Controls::bind(int key, Action action){
    actions[key] = std::move(action);
}

void Controls::onKey(int key, int glfwAction){
    if (glfwAction == GLFW_REPEAT) return;   // auto-repeat, ignoring 
        bool pressed = (glfwAction == GLFW_PRESS);
        down[key] = pressed;
        if (pressed) {
            auto it = actions.find(key);
            if (it != actions.end()) it->second();
        }
}

bool Controls::isDown(int key) const {
        auto it = down.find(key);
        return it != down.end() && it->second;
}