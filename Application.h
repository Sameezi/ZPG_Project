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

	private:
		GLFWwindow* window{nullptr};
		Scene* main_scene;
		std::vector<Scene*> m_scenes;
		int currentScene{0};
};

