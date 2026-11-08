#include "Window.h"


Window::Window(int width, int height, const char* name){
	glfwSetErrorCallback([](int code, const char* desc) {
    std::cerr << "GLFW error " << code << ": " << desc << std::endl;
	});

	if (!glfwInit()) throw std::runtime_error("glfwInit failed");

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	this->handle = glfwCreateWindow(width,height,name,NULL,NULL);
	if (!handle){
		glfwTerminate();
        throw std::runtime_error("window creation failed");
	}

	glfwMakeContextCurrent(handle);
    glfwSwapInterval(1);
}

Window::~Window(){
    if (handle != nullptr) {
		glfwDestroyWindow(handle);
	}
	glfwTerminate();
}
