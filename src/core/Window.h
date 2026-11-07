#pragma once


#include <glad/gl.h>
#include <GLFW/glfw3.h>  
#include <stdexcept>
#include <iostream>

class Window{
    public:
        Window(int width, int height, const char* name);
        ~Window();
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        GLFWwindow* get() const { return handle; }
    private:
        GLFWwindow* handle{nullptr};
};