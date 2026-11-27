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
#include "Controls.h"

class Application{
	public:
		Application();
		~Application();
		int init();
		int keybinds();
		void run();
		void addScenes();
		void processInput();
		float secondsSinceMidnight();

		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void cursorPosCallback(GLFWwindow* window, double x, double y);
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
		void onResize(int width, int height);
		void onPreviewObject(glm::vec3 position);
		void onClickSelectObject(GLuint id);

	private:
		Window window;	
		Resources resources;
		Scene* main_scene;
		std::vector<Scene*> m_scenes;
		Controls m_keyboard;
		int currentScene{0};

		bool m_dragging{false};
		double m_lastX{0.0};
		double m_lastY{0.0};
		int fbWidth = 1366, fbHeight = 768;
		GLuint selectedId = -1;

		glm::vec3 getWorldPosition();
};

