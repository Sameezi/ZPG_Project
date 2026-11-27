/**
 * @file main.cpp
 *
 * @brief Main function
 *
 * @author KUC0438 
  **/


#include "core/Application.h"

int main(void)
{
	Application* app = new Application();
	if (app->init() != 0) {
		return EXIT_FAILURE;
	}
	app->addScenes();
	app->keybinds();
	app->run();
	return EXIT_SUCCESS;
}


