/**
 * @file main.cpp
 *
 * @brief Main function
 *
 * @author KUC0438 
  **/


#include "Application.h"

int main(void)
{
	Application* app = new Application();
	if (app->init() != 0) {
		return EXIT_FAILURE;
	}
	app->addScenes();
	app->run();
	return EXIT_SUCCESS;
}


