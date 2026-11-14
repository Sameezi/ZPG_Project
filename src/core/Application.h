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

#include "Window.h"
#include "Scene.h"

#include "ResourceManager.hpp"
#include "parametricAxis.h"


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
		void onClickAddObject();
		void onPreviewObject(glm::vec3 position);
		void onClickDestObject(GLuint id);
		void onClickSelectObject(GLuint id);
		GLuint getSelectedId();
		int getMode();

	private:
		Window window;	
		Resources resources;
		Scene* main_scene;
		std::vector<Scene*> m_scenes;
		int currentScene{0};

		bool m_dragging{false};
		bool k_pressed{false};
		bool arrows[2]{false};
		double m_lastX{0.0};
		double m_lastY{0.0};
		int fbWidth = 1366, fbHeight = 768;
		GLuint selectedId = -1;

		std::string mode{"Default"};
		int modInx = 0;
		glm::vec4 rotation{0.f, 0.f, 1.f, 0.f};
		glm::vec3 getWorldPosition();
};

