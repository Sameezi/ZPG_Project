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

#include "graphics/ShaderProgram.h"
#include "Application.h"
#include "../models/tree.h"
#include "../models/earth.h"
#include "../models/moon.h"
#include "../models/sun.h"
#include "../models/clock.h"
#include "../models/login_points.h"
#include "../models/suzi_flat.h"
#include "../models/sphere.h"
#include "../models/rocket.h"
#include "transformation/Scaling.h"
#include "transformation/Translation.h"
#include "transformation/Rotation.h"
#include "transformation/TransformComposite.h"
#include "core/Camera.h"
#include "graphics/Model.h"
#include "graphics/InstancedModel.h"

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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	this->window = glfwCreateWindow(800,600,"ZPG",NULL,NULL);
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowUserPointer(window,this);
	glfwSetMouseButtonCallback(window, Application::mouseButtonCallback);
	glfwSetCursorPosCallback(window, Application::cursorPosCallback);
	glfwSetFramebufferSizeCallback(window, Application::framebufferSizeCallback);

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

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app || button != GLFW_MOUSE_BUTTON_RIGHT) return;

    if (action == GLFW_PRESS) {
        app->m_dragging = true;
    glfwGetCursorPos(window, &app->m_lastX, &app->m_lastY);
    } else if (action == GLFW_RELEASE) {
        app->m_dragging = false;
    }
}

void Application::cursorPosCallback(GLFWwindow* window, double x, double y) {
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app || !app->m_dragging) return;

    const float sensitivity = 0.1f;
    float dx = (float)(x - app->m_lastX) * sensitivity;
    float dy = (float)(app->m_lastY - y) * sensitivity;   
    app->m_lastX = x;
    app->m_lastY = y;

    app->main_scene->getCamera()->rotate(dx, dy);
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height){
	auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) app->onResize(width, height);
}

void Application::onResize(int width, int height)
{
    if (width == 0 || height == 0) return; // minimized

    fbWidth = width;
    fbHeight = height;
    glViewport(0, 0, width, height);

	for(auto& scene: m_scenes){
		scene->getCamera()->setAspectRatio(static_cast<float>(width) / height);
	}

}

void Application::processInput(){

	float cameraSpeed = 0.05f;
	bool shiftHeld = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                 glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

	if (shiftHeld) {
	    cameraSpeed = .50f; 
	} else {
	    cameraSpeed = .05f;
	}

	glm::vec2 movement{0.0f, 0.0f};
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movement.x += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movement.x -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movement.y += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movement.y -= 1.0f;

	if (glm::length(movement) > 0.0f) {
        movement = glm::normalize(movement);
    }

	main_scene->getCamera()->setMovement(movement * cameraSpeed);

	if(glfwGetKey(window,GLFW_KEY_1) == GLFW_PRESS){
		glm::vec4 backgroundColor = this->main_scene->getBackgroundColor();
	 	glClearColor(backgroundColor.x,backgroundColor.y,backgroundColor.z,backgroundColor.w);
		this->main_scene = m_scenes[0];
	}
	if(glfwGetKey(window,GLFW_KEY_2) == GLFW_PRESS){
		glm::vec4 backgroundColor = this->main_scene->getBackgroundColor();
    	glClearColor(backgroundColor.x,backgroundColor.y,backgroundColor.z,backgroundColor.w);
		this->main_scene = m_scenes[1];
	}
	if(glfwGetKey(window,GLFW_KEY_3) == GLFW_PRESS){
		glm::vec4 backgroundColor = this->main_scene->getBackgroundColor();
    	glClearColor(backgroundColor.x,backgroundColor.y,backgroundColor.z,backgroundColor.w);
		this->main_scene = m_scenes[2];
	}
}

float Application::randomFloat(float lo, float hi) {
     static std::mt19937 gen{std::random_device{}()};
     std::uniform_real_distribution<float> dist(lo, hi);
     return dist(gen);
}

float Application::secondsSinceMidnight()
{
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);

    std::tm lt;
    localtime_r(&t, &lt);                       // Linux/Debian; use localtime_s on Windows
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    double s = lt.tm_hour * 3600.0 + lt.tm_min * 60.0 + lt.tm_sec
             + ms.count() / 1000.0;             // fractional part = smooth sweep
    return static_cast<float>(s);
}


void Application::addScenes(){
	 // Space example
	 m_scenes.push_back(new Scene());
	 // Forest example
	 m_scenes.push_back(new Scene());
	 // Lighting example
	 m_scenes.push_back(new Scene());

	 ///////////////////////////////// Setting up space scene
	 m_scenes[0]->init();
     std::vector<float> earthVertices(earth, earth+ (sizeof(earth) / sizeof(float)));
     std::vector<float> sunVertices(sun,sun + (sizeof(sun) / sizeof(float)));
     std::vector<float> moonVertices(moon,moon + (sizeof(moon) / sizeof(float)));
	 std::vector<float> loginVertices(login,login + (sizeof(login) / sizeof(float)));
	 std::vector<float> rocketVertices(rocket, rocket + (sizeof(rocket) / sizeof(float)));
	 auto phongShader = std::make_shared<ShaderProgram>
	 (
		"shaders/lighting_9float.vert",
		"shaders/phong_point.frag",
		m_scenes[0]->getCamera()
	 );
	 auto ambientShader = std::make_shared<ShaderProgram>
	 (
		"shaders/lighting_9float.vert",
		"shaders/ambient.frag",
		m_scenes[0]->getCamera()
	 );
	 auto blinPhongShader = std::make_shared<ShaderProgram>
	 (
		"shaders/lighting_6float.vert",
		"shaders/phong_color.frag",
		m_scenes[0]->getCamera()
	 );
     // Creating sun
	 m_scenes[0]->addObject("sun",std::make_unique<DrawableObject>(std::make_shared<Model>(sunVertices,9),ambientShader));
	 // Creating earth
	 m_scenes[0]->addObject("moon",std::make_unique<DrawableObject>(std::make_shared<Model>(moonVertices,9),phongShader));
	 // Creating moon
	 m_scenes[0]->addObject("earth",std::make_unique<DrawableObject>(std::make_shared<Model>(earthVertices,9),phongShader));
	 // Creating login
	 m_scenes[0]->addObject("login",std::make_unique<DrawableObject>(std::make_shared<Model>(loginVertices,6),blinPhongShader));
	 // Creating rocket
	 m_scenes[0]->addObject("rocket",std::make_unique<DrawableObject>(std::make_shared<Model>(rocketVertices,9),phongShader));
	{												
	//sun 
	auto sun = std::make_shared<TransformComposite>();
	sun->addNode(std::make_shared<Rotation>(glm::radians(70.f), glm::vec3(0.f, -1.f, 0.f), 0.3f));
	sun->addNode(std::make_shared<Scaling>(.3f));
	m_scenes[0]->getObject("sun").setTransform(sun);

	// earth orbit around sun
	auto earthOrbit = std::make_shared<TransformComposite>();
	earthOrbit->addNode(std::make_shared<Rotation>(0.f, glm::vec3(0.f, 1.f, 0.f), 0.5f));
	earthOrbit->addNode(std::make_shared<Translation>(glm::vec3(0.9f, 0.f, 0.f)));

	//  earth * scale
	auto earth = std::make_shared<TransformComposite>();
	earth->addNode(earthOrbit);
	earth->addNode(std::make_shared<Rotation>(glm::radians(70.f), glm::vec3(0.f, 1.f, 0.f), 8.0f));
	earth->addNode(std::make_shared<Scaling>(.03f));
	m_scenes[0]->getObject("earth").setTransform(earth);

	// moon =earth orbit * orbit around earth * scale 
	auto moon = std::make_shared<TransformComposite>();
	moon->addNode(earthOrbit);                                                          
	moon->addNode(std::make_shared<Rotation>(0.f, glm::vec3(0.f, 1.f, 0.f), 2.f));        
	moon->addNode(std::make_shared<Translation>(glm::vec3(0.2f, 0.f, 0.f)));              
	moon->addNode(std::make_shared<Scaling>(.01f));
	m_scenes[0]->getObject("moon").setTransform(moon);

	auto login = std::make_shared<TransformComposite>();
	login->addNode(std::make_shared<Translation>(glm::vec3(1.0f, 1.0f, 1.0f)));              
	login->addNode(std::make_shared<Rotation>(0.f, glm::vec3(0.f, 1.f, 0.f), 2.f));        
	login->addNode(std::make_shared<Scaling>(.25f));
	m_scenes[0]->getObject("login").setTransform(login);

	// earth orbit around sun
	auto shipOrbit = std::make_shared<TransformComposite>();
	shipOrbit->addNode(std::make_shared<Rotation>(0.f, glm::vec3(0.f, -1.f, 0.f), 0.5f));
	shipOrbit->addNode(std::make_shared<Translation>(glm::vec3(1.9f, 0.f, 0.f)));

	auto rocket = std::make_shared<TransformComposite>();
	rocket->addNode(shipOrbit);
	rocket->addNode(std::make_shared<Rotation>(0.f, glm::vec3(0.f, 0.f, 1.f),1.0f));        
	rocket->addNode(std::make_shared<Scaling>(.00009f));
	m_scenes[0]->getObject("rocket").setTransform(rocket);
	}

	 ///////////////////////////////// Setting up forest scene
	 m_scenes[1]->init();
	 
	 { // Creating ground
	 glm::vec3 groundColor = {0.16f, 0.32f, 0.0f};
	 std::vector<float> groundVertices = {
		0.0f, 0.5f, 1.0f, groundColor.x,groundColor.y,groundColor.z,
		-0.5f, 0.5f, 1.0f, groundColor.x,groundColor.y,groundColor.z,
		-0.5f, 0.0f, 1.0f,groundColor.x,groundColor.y,groundColor.z, 
		0.0f, 0.0f, 1.0f,groundColor.x,groundColor.y,groundColor.z, 
		0.0f, 0.5f, 1.0f,groundColor.x,groundColor.y,groundColor.z, 
		-0.5f, 0.0f, 1.0f,groundColor.x,groundColor.y,groundColor.z 
	 };
	 auto composite = std::make_shared<TransformComposite>();
	 composite->addNode(std::make_shared<Translation>(glm::vec3(25.0f,100.0f,-25.0f)));
	 composite->addNode(std::make_shared<Rotation>(glm::radians(90.0f),glm::vec3(1.0f,0.0f,0.0f)));
	 composite->addNode(std::make_shared<Scaling>(100.0f));
	 m_scenes[1]->addObject("ground", 
		std::make_unique<DrawableObject>(std::make_shared<Model>(groundVertices),
		std::make_shared<ShaderProgram>("shaders/projective_6float.vert","shaders/basic.frag",m_scenes[1]->getCamera()))	
	 ).setTransform(composite);
	 }

	 {// Creating nicer trees
	 std::vector<float> treeVertices(tree, tree+ (sizeof(tree) / sizeof(float)));
	 auto niceTree = std::make_shared<ShaderProgram>(
    	"shaders/treeColored.vert",
    	"shaders/treeColored.frag",
		m_scenes[1]->getCamera()
	 );
	 auto treeModel = std::make_shared<InstancedModel>(treeVertices);
	 std::vector<TransformComposite> composites;
	 for(int i = 0; i <= 73; i++){
	 float randomScale = randomFloat(0.1f, 0.25f); 
	 float randomZ = randomFloat(-25.0f, 25.0f);
	 float randomX = randomFloat(-25.0f, 25.0f);
	 float randomRot = randomFloat(-90.0f,90.0f);

	 TransformComposite composite; 
	 
	 composite.addNode(std::make_shared<Translation>(glm::vec3(randomX, .0f, randomZ)));
	 composite.addNode(std::make_shared<Rotation>(glm::radians(randomRot),glm::vec3(0.0,1.0,0.0)));
	 composite.addNode(std::make_shared<Scaling>(randomScale));
	 composites.push_back(std::move(composite));
	 }
	 treeModel->setInstances(composites);
	 m_scenes[1]->addObject("trees",std::make_unique<DrawableObject>(treeModel, niceTree));
	 }

	 // Adding clock
	 {
	 std::vector<float> clockVertices(clockModel, clockModel + sizeof(clockModel) / sizeof(float));
	 auto shader = std::make_shared<ShaderProgram>("shaders/clock.vert","shaders/clock.frag",m_scenes[1]->getCamera());
	 auto model = std::make_shared<Model>(clockVertices,9);
	 auto composite = std::make_shared<TransformComposite>();
	 composite->addNode(std::make_shared<Translation>(glm::vec3(.0f,2.0f,.0f)));
	 composite->addNode(std::make_shared<Scaling>(glm::vec3(-1,1,1)));
	 m_scenes[1]->addObject("clock1", std::make_unique<DrawableObject>(model,shader)).setTransform(composite);

	 auto composite2 = std::make_shared<TransformComposite>();
	 composite2->addNode(std::make_shared<Translation>(glm::vec3(2.0f,2.0f,.0f)));
	 composite2->addNode(std::make_shared<Scaling>(glm::vec3(-1,1,1)));
	 m_scenes[1]->addObject("clock2", std::make_unique<DrawableObject>(model,shader)).setTransform(composite2);
	 }
	 
	 this->main_scene = m_scenes[1];
		glm::mat4 proj = glm::perspective(glm::radians(60.0f),
                                800.0f / 600.0f,
                                0.1f, 100.0f);

	/////////// Setting up light example scene 
	m_scenes[2]->init();
	 {
		std::vector<float> sphereVertices(sphere, sphere+ (sizeof(sphere) / sizeof(float)));
		auto shader = std::make_shared<ShaderProgram>("shaders/lighting_6float.vert","shaders/blinn_phong.frag",m_scenes[2]->getCamera());

		auto model = std::make_shared<Model>(sphereVertices);
		
		float coords[8] = {1.0f,0.0f,-1.0f,0.0f,0.0f,1.0f,0.0f,-1.0f};
		int idx = 0;
		for(int i = 0; i <= 4; i++){
			std::string name = "sphere" + std::to_string(i);
			auto composite = std::make_shared<TransformComposite>();
			int x = coords[idx++];
			int y = coords[idx++];
			composite->addNode(std::make_shared<Translation>(glm::vec3(x,y,0.0f)));
			composite->addNode(std::make_shared<Scaling>(0.25f));
			m_scenes[2]->addObject(name, std::make_unique<DrawableObject>(model,shader)).setTransform(composite);
		}
	 }

	 this->main_scene = m_scenes[0];
	for(auto& scene: m_scenes){
		scene->getCamera()->setProjMatrix(proj);
	}
	
	
}


void Application::run(){
	double lastTime = glfwGetTime();
	int frameCount = 0;
	glEnable(GL_MULTISAMPLE);
	while (!glfwWindowShouldClose(window))
	{
	// FPS
	double now = glfwGetTime();
	frameCount++;
	
	float deltaTime = now - lastTime;
    if (deltaTime >= 1.0) {                 // once per second 
        double fps = frameCount / (now - lastTime);
        std::string title = "ZPG - " + std::to_string(static_cast<int>(fps)) + " FPS";
        glfwSetWindowTitle(window, title.c_str());
        frameCount = 0;
        lastTime = now;
    }
	if(main_scene == m_scenes[1]) main_scene->getObject("clock1").setTime(secondsSinceMidnight());
	
	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	main_scene->getCamera()->update(1.0f);
	processInput();

	// Reset
	RenderStats::drawCalls = 0;
	RenderStats::instances = 0;

	// Draw scene
	glEnable(GL_DEPTH_TEST);
	main_scene->draw();

		std::cout << "draw calls: " << RenderStats::drawCalls
              << ", instances: " << RenderStats::instances << '\n';
	
	// Display the rendered frame and process events
	glfwSwapBuffers(window);
	glfwPollEvents();
	}
}