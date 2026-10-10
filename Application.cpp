/**
 * @file Application.cpp
 *
 * @brief Application processor 
 *
 * @author KUC0438 
  **/
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>  

#include <random>

#include "ShaderProgram.h"
#include "Application.h"
#include "models/tree.h"
#include "models/sphere.h"
#include "models/earth.h"
#include "models/moon.h"
#include "models/sun.h"

Application::Application() = default;

Application::~Application(){
	if (window != nullptr) {
		glfwDestroyWindow(window);
	}
	glfwTerminate();
	delete(this->main_scene);
}

int Application::init(){
	// Initialize GLFW
	if (!glfwInit()){
		exit(EXIT_FAILURE);
	}
	
	/*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,
	GLFW_OPENGL_CORE_PROFILE);  //*/

	// Create a window
	this->window = glfwCreateWindow(800,600,"ZPG",NULL,NULL);
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	// Initialize GLAD and load OpenGL function pointers
	if (!gladLoadGL(glfwGetProcAddress)) {
		std::cout << "Failed to inititalize GLAD" << std::endl;
		glfwDestroyWindow(window);
		window = nullptr;
		glfwTerminate();
		return -1;
	}


	// Get version info
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("Vendor %s\n", glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Using GLFW %i.%i.%i\n", major, minor, revision);
	return 0;
}

void Application::processInput(){
	
	if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS){
	}


	if(glfwGetKey(window,GLFW_KEY_1) == GLFW_PRESS){
		this->main_scene = m_scenes[0];
	}
	if(glfwGetKey(window,GLFW_KEY_2) == GLFW_PRESS){
		this->main_scene = m_scenes[1];
	}
	if(glfwGetKey(window,GLFW_KEY_3) == GLFW_PRESS){
		this->main_scene = m_scenes[2];
	}
}


void Application::addScenes(){
	 // Triangle example
	 m_scenes.push_back(new Scene());
	 // Space example 
	 m_scenes.push_back(new Scene());
	 // Forest example
	 m_scenes.push_back(new Scene());


	 // Setting up space scene
	 m_scenes[0]->init();
     std::vector<float> earthVertices(earth, earth+ (sizeof(earth) / sizeof(float)));
     std::vector<float> sunVertices(sun,sun + (sizeof(sun) / sizeof(float)));
     std::vector<float> moonVertices(moon,moon + (sizeof(moon) / sizeof(float)));
	 
	 // Creating sun
	 m_scenes[0]->addObject("sun",std::make_unique<DrawableObject>(sunVertices,9,
															std::make_unique<ShaderProgram>("shaders/transformable_9float.vert","shaders/basic.frag")));
	 m_scenes[0]->getObject("sun").getTransform().setScale(0.2f);														

	// Creating earth
	 m_scenes[0]->addGroup("earthPivot"); // Pivot rotating around sun
	 m_scenes[0]->addGroup("earthSystem","earthPivot").getTransform().setPosition({0.9f,0.0f,0.0f}); // Offset from sun
	 
	 m_scenes[0]->addObject("earth",std::make_unique<DrawableObject>(earthVertices,9,
															std::make_unique<ShaderProgram>("shaders/transformable_9float.vert","shaders/basic.frag")),"earthSystem");
	 m_scenes[0]->getObject("earth").getTransform().setScale(0.03f);
	 
	 // Creating moon
	 m_scenes[0]->addGroup("moonPivot","earthSystem");
	 auto& moon = m_scenes[0]->addObject("moon",std::make_unique<DrawableObject>(moonVertices,9,
															std::make_unique<ShaderProgram>("shaders/transformable_9float.vert","shaders/basic.frag")),"moonPivot");
	 moon.getTransform().setPosition({0.1f,0.0f,0.0f});
	 moon.getTransform().setScale(0.01f);


	 // Setting up triangle scene
	 m_scenes[1]->init();
	 float triangle[]={-1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
					    0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,
						1.0f,0.0f,0.0f,0.0f,0.0f,1.0f};
	 std::vector<float> triangleVertices(triangle, triangle+ (sizeof(triangle) / sizeof(float)));
	 m_scenes[1]->addObject("triangle",std::make_unique<DrawableObject>(triangleVertices,
															std::make_unique<ShaderProgram>("shaders/transformable.vert","shaders/basic.frag")));

	 // Setting up forest scene
	 m_scenes[2]->init();
	 std::vector<float> treeVertices(tree, tree+ (sizeof(tree) / sizeof(float)));

	 for(int i = 0; i <= 3; i++){
     std::string name = "tree" + std::to_string(i);
	 std::random_device rd;
	 std::mt19937 gen(rd());
	 std::uniform_real_distribution<float> dist(0.1f, 0.25f);
	 float randomScale = dist(gen);
	 m_scenes[2]->addObject(name,std::make_unique<DrawableObject>(treeVertices,std::make_unique<ShaderProgram>("shaders/transformable.vert","shaders/basic.frag")));
	 m_scenes[2]->getObject(name).getTransform().setScale(0.25f);
	 m_scenes[2]->getObject(name).getTransform().setPosition({randomScale*3,0.0f,0.0f});
	 }

	 this->main_scene = m_scenes[0];
	 glm::vec4 backgroundColor = this->main_scene->getBackgroundColor();
	 glClearColor(backgroundColor.x,backgroundColor.y,backgroundColor.z,backgroundColor.w);
}


void Application::run(){
	double lastTime = glfwGetTime();
	int frameCount = 0;

	while (!glfwWindowShouldClose(window))
	{
	// FPS
	double now = glfwGetTime();
	frameCount++;

    if (now - lastTime >= 1.0) {                 // once per second 
        double fps = frameCount / (now - lastTime);
        std::string title = "ZPG - " + std::to_string(static_cast<int>(fps)) + " FPS";
        glfwSetWindowTitle(window, title.c_str());
        frameCount = 0;
        lastTime = now;
    }

	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	processInput();

	glEnable(GL_DEPTH_TEST);
	main_scene->draw();
	if(main_scene == m_scenes[0]){
		main_scene->getObject("earth").getTransform().setRotation({23.5f,now*32,0});
		main_scene->getObject("sun").getTransform().setRotation({0,now*(-2),0});
		main_scene->getGroup("earthPivot").getTransform().setRotation({0, now * 32.0f,  0});
		main_scene->getGroup("moonPivot").getTransform().setRotation({0, now * 120.0f, 0});
	}
	// Display the rendered frame and process events
	glfwSwapBuffers(window);
	glfwPollEvents();
	}
}

