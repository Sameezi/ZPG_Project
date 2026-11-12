#pragma once


//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iterator>
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <tuple>

#include "Window.h"
#include "Scene.h"

#include "ResourceManager.hpp"


struct Resources {
    ResourceManager<ShaderProgram> shaders;
    ResourceManager<Texture>       textures;
    ResourceManager<Cubemap>       cubemaps;
    ResourceManager<Model>         models;
	ResourceManager<InstancedModel> imodels;
};

class Application{
	public:
		Application();
		~Application();
		int init();
		void run();
		void addScenes();
		void processInput();
		float randomFloat(float lo, float hi);
		float secondsSinceMidnight();

		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void cursorPosCallback(GLFWwindow* window, double x, double y);
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
		void onResize(int width, int height);
		void onClickAddObject(glm::vec3 position);
		void onPreviewObject(glm::vec3 position);
		void onClickDestObject(GLuint id);
		int getMode();

	private:
		Window window;	
		Resources resources;
		Scene* main_scene;
		std::vector<Scene*> m_scenes;
		int currentScene{0};

		bool m_dragging{false};
		bool k_pressed{false};
		double m_lastX{0.0};
		double m_lastY{0.0};
		int fbWidth = 1366, fbHeight = 768;
		glm::vec3 lightPosition{0.0f};
		std::string mode{"Default"};
		std::tuple<std::string, std::string, glm::vec4> editorObject; // <model name, shaderProgram name, rotation>

		glm::vec3 getWorldPosition(int width, int height, double xpos, double ypos);
};

