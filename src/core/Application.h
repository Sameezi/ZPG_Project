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

#include "Scene.h"

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
	private:
		GLFWwindow* window{nullptr};
		Scene* main_scene;
		std::vector<Scene*> m_scenes;
		int currentScene{0};

		bool m_dragging{false};
		double m_lastX{0.0};
		double m_lastY{0.0};
		int fbWidth = 800, fbHeight = 600;
		glm::vec3 lightPosition{0.0f};

};

