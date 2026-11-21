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
#include "graphics/Cubemap.h"
#include "graphics/Skybox.h"
#include "Logger.hpp"
#include "graphics/lights/SpotLight.h"

Application::Application() :  window(1366,768,"ZPG") {}

Application::~Application(){
	for(auto& scene: m_scenes)
		delete(scene);
}

int Application::init(){
	Logger::Init();

	GLFWwindow* win = window.get();
	glfwSetWindowUserPointer(win,this);
	glfwSetMouseButtonCallback(win, Application::mouseButtonCallback);
	glfwSetCursorPosCallback(win, Application::cursorPosCallback);
	glfwSetFramebufferSizeCallback(win, Application::framebufferSizeCallback);

	// Initialize GLAD and load OpenGL function pointers
	if (!gladLoadGL(glfwGetProcAddress)) {
		 throw std::runtime_error("Failed to initialize GLAD");
	}

	// Get version info
	int a;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &a);
	Logger::Debug("Texture units: " , a);
	Logger::Info("OpenGL Version: ", glGetString(GL_VERSION));
	Logger::Info("Vendor " , glGetString(GL_VENDOR));
	Logger::Info("Renderer " , glGetString(GL_RENDERER));
	Logger::Debug("GLSL " , glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	Logger::Info("Using GLFW " , major , ". " , minor , ". " , revision);
	return 0;
}

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app) return;

	if((action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)){ 
		// getting cursor position
		double xpos, ypos;
   		glfwGetCursorPos(window, &xpos, &ypos);
   		GLint x = static_cast<GLint>(xpos);
   		GLint y = static_cast<GLint>(ypos);

		// getting window dimensions
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// getting id under cursor pos
		GLuint id = 0;
		glReadPixels(x, height - y - 1, 1, 1,
             GL_STENCIL_INDEX, GL_UNSIGNED_INT, &id);

		if(app->getMode() == 0){
			// selection range only from 1-255
			if (id != 0){
				std::cout << "Clicked on object: " << id << std::endl;
				app->onClickSelectObject(id);
			}
		}
		if(app->getMode() == 1) 
			app->onClickAddObject();
		if(app->getMode() == 2)
			app->onClickDestObject(id);
		if(app->getMode() == 3){
			app->main_scene->getObject(app->getSelectedId()).setParametricAxis(app->getWorldPosition()); // get last added object and set it's parametric axis
		}
	}
	
	// Movement camera around
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT) {
		app->m_dragging = true;
		glfwGetCursorPos(window, &app->m_lastX, &app->m_lastY);
	} else if (action == GLFW_RELEASE) {
		app->m_dragging = false;
	}
}

void Application::cursorPosCallback(GLFWwindow* window, double x, double y) {
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app) return;

	if(app->m_dragging){
		const float sensitivity = 0.1f;
		float dx = (float)(x - app->m_lastX) * sensitivity;
		float dy = (float)(app->m_lastY - y) * sensitivity;   
		app->m_lastX = x;
		app->m_lastY = y;

		app->main_scene->getCamera()->rotate(dx, dy);
	}
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

glm::vec3 Application::getWorldPosition(){
	GLFWwindow* win = window.get();
	// getting cursor position
		double xpos, ypos;
   		glfwGetCursorPos(win, &xpos, &ypos);
   		GLint x = static_cast<GLint>(xpos);
   		GLint y = static_cast<GLint>(ypos);

	// getting window dimensions
		int width, height;
		glfwGetFramebufferSize(win, &width, &height);

	float winX = static_cast<float>(xpos);
	float winY = static_cast<float>(height) - static_cast<float>(ypos) - 1.0f;
	// getting depth from depth buffer
	float depth;
	glReadPixels((GLint)winX, (GLint)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	// reconstructing point from screen space to world space
	glm::vec4 viewport(0, 0, width, height);
	glm::mat4 view = main_scene->getCamera()->getViewMatrix();
	glm::mat4 projection = main_scene->getCamera()->getProjMatrix();
	glm::vec3 worldPos = glm::unProject(glm::vec3(winX, winY, depth), view, projection, viewport);
	return worldPos;
}

void Application::onClickAddObject(){
	main_scene->addObject(main_scene->getEditorObjModel(),*resources.shaders.get("texture"));
	main_scene->getObject().setTransform(main_scene->getEditorObjTransform());
}

void Application::onPreviewObject(glm::vec3 position){
	main_scene->setEditorObjTranslation(position);
}

void Application::onClickDestObject(GLuint id){
	if(main_scene->removeObject(id)) // Only deletes user made objects starting with "obj-" + ID
		std::cout << "Destroyed user object " << id << std::endl;
}

void Application::onClickSelectObject(GLuint id){
	selectedId = id;	
}

GLuint Application::getSelectedId(){
	return selectedId;
}

int Application::getMode(){
	if(mode.compare("Default") == 0){
		return 0;
	}else if(mode.compare("Insert") == 0){
		return 1;
	}else if(mode.compare("Delete") == 0){
		return 2;
	}else if(mode.compare("Parametric axis") == 0){
		return 3;
	}else{
		return 4;
	}
}

void Application::processInput(){

	GLFWwindow* win = window.get();
	float cameraSpeed = 0.05f;
	bool shiftHeld = glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                 glfwGetKey(win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

	if (shiftHeld) {
	    cameraSpeed = .50f; 
	} else {
	    cameraSpeed = .05f;
	}

	glm::vec2 movement{0.0f, 0.0f};
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) movement.x += 1.0f;
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) movement.x -= 1.0f;
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) movement.y += 1.0f;
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) movement.y -= 1.0f;

	if (glm::length(movement) > 0.0f) {
        movement = glm::normalize(movement);
    }

	main_scene->getCamera()->setMovement(movement * cameraSpeed);
	
	float speed = glm::radians(10.0f);   
	if (glfwGetKey(win, GLFW_KEY_LEFT)  == GLFW_PRESS) {
		rotation.x += speed;
		main_scene->setEditorObjRotation(rotation);
	}
	if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotation.x -= speed;
		main_scene->setEditorObjRotation(rotation);
	}	

	if (glfwGetKey(win, GLFW_KEY_UP)  == GLFW_PRESS && !arrows[0]) {
			if(++modInx > resources.models.size()-1) modInx = 0;
    		main_scene->setEditorObjModel(resources.models.at(modInx));
			arrows[0] = true;
	}
	if (glfwGetKey(win, GLFW_KEY_DOWN)  == GLFW_PRESS && !arrows[1]) {
			if(modInx < 0) modInx = resources.models.size()-1;
    		main_scene->setEditorObjModel(resources.models.at(modInx--));
			arrows[1] = true;
	}
	if (glfwGetKey(win,GLFW_KEY_UP) == GLFW_RELEASE) arrows[0] = false;
	if (glfwGetKey(win,GLFW_KEY_DOWN) == GLFW_RELEASE) arrows[1] = false;

	if(glfwGetKey(win,GLFW_KEY_1) == GLFW_PRESS){
		glm::vec4 backgroundColor = this->main_scene->getBackgroundColor();
	 	glClearColor(backgroundColor.x,backgroundColor.y,backgroundColor.z,backgroundColor.w);
		this->main_scene = m_scenes[0];
	}
	if(glfwGetKey(win,GLFW_KEY_2) == GLFW_PRESS){
		glm::vec4 backgroundColor = this->main_scene->getBackgroundColor();
    	glClearColor(backgroundColor.x,backgroundColor.y,backgroundColor.z,backgroundColor.w);
		this->main_scene = m_scenes[1];
	}
	if(glfwGetKey(win,GLFW_KEY_3) == GLFW_PRESS){
		glm::vec4 backgroundColor = this->main_scene->getBackgroundColor();
    	glClearColor(backgroundColor.x,backgroundColor.y,backgroundColor.z,backgroundColor.w);
		this->main_scene = m_scenes[2];
	}
	if(glfwGetKey(win,GLFW_KEY_TAB) == GLFW_PRESS && k_pressed == false){
		if(mode.compare("Default") == 0){
			mode = "Insert";
		}else if(mode.compare("Insert") == 0){
			mode = "Delete";
		}else if(mode.compare("Delete") == 0){
			mode = "Parametric axis";
		}else{
			mode = "Default";
		}
		k_pressed = true;
	}
	if(glfwGetKey(win,GLFW_KEY_TAB) == GLFW_RELEASE)
		k_pressed = false;

	if(glfwGetKey(win,GLFW_KEY_F) == GLFW_PRESS){
		m_scenes[1]->getLightObject("flashlight").enable();
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

	 std::vector<float> earthVertices(earth, earth+ (sizeof(earth) / sizeof(float)));
     std::vector<float> sunVertices(sun,sun + (sizeof(sun) / sizeof(float)));
     std::vector<float> moonVertices(moon,moon + (sizeof(moon) / sizeof(float)));
	 std::vector<float> loginVertices(login,login + (sizeof(login) / sizeof(float)));
	 std::vector<float> rocketVertices(rocket, rocket + (sizeof(rocket) / sizeof(float)));
	 std::vector<float> sphereVertices(sphere, sphere+ (sizeof(sphere) / sizeof(float)));
	 std::vector<float> treeVertices(tree, tree+ (sizeof(tree) / sizeof(float)));

     glm::vec3 n(0.0f, 0.0f, 1.0f);
	 glm::vec3 groundColor = {0.16f, 0.32f, 0.0f};
	 std::vector<float> groundVertices = {
     0.0f,  0.5f, 1.0f,   n.x, n.y, n.z,   0.0f, 1.0f,
    -0.5f,  0.5f, 1.0f,   n.x, n.y, n.z,   1.0f, 1.0f,
    -0.5f,  0.0f, 1.0f,   n.x, n.y, n.z,   1.0f, 0.0f,
     0.0f,  0.0f, 1.0f,   n.x, n.y, n.z,   0.0f, 0.0f,
     0.0f,  0.5f, 1.0f,   n.x, n.y, n.z,   0.0f, 1.0f,
    -0.5f,  0.0f, 1.0f,   n.x, n.y, n.z,   1.0f, 0.0f
	};

	 auto& shaderManager = resources.shaders;
	 auto& phong = shaderManager.load("phong",  "shaders/lighting_9float.vert",  "shaders/phong.frag");
	 auto& phongTex = shaderManager.load("phongTex",  "shaders/lighting_texture.vert",  "shaders/phong_textures.frag");
	 auto& ambient = shaderManager.load("ambient", "shaders/lighting_9float.vert", "shaders/ambient.frag");
	 auto& blinPhong = shaderManager.load("blinphong", "shaders/lighting_9float.vert", "shaders/blinn_phong.frag");
	 auto& blinPhong_2 = shaderManager.load("blinphong_2", "shaders/lighting_6float.vert", "shaders/blinn_phong.frag");
	 auto& diffuse = shaderManager.load("diffuse","shaders/lighting_6float.vert","shaders/diffuse.frag");
	 auto& ambient6 = shaderManager.load("ambient6","shaders/lighting_6float.vert","shaders/ambient.frag");
	 auto& diffuseTex = shaderManager.load("diffuseTex","shaders/lighting_texture.vert","shaders/diffuse_texture.frag");
     auto& textureShader = shaderManager.load("texture","shaders/texture.vert","shaders/texture.frag");
	 auto& lightsShader = shaderManager.load("lights", "shaders/lighting_texture.vert","shaders/phong_texture_ubo.frag");
	 auto& lightsShaderUntex = shaderManager.load("lightsUntex", "shaders/lighting_9float.vert","shaders/phong_ubo.frag");
	 auto& treecolor = shaderManager.load("treecolor","shaders/treeColored.vert","shaders/phong_ubo.frag");
	 auto& skyShader = resources.shaders.load("skybox", "shaders/skybox.vert", "shaders/skybox.frag");

	 auto& modelManager = resources.models;
	 auto& shrek = modelManager.load("shrek","../models/shrek.obj","../textures/shrek.png");
	 auto& fiona = modelManager.load("fiona","../models/fiona.obj","../textures/fiona.png");
	 auto& ground = modelManager.load("ground","../models/teren.obj","../textures/grass.jpg");
	 auto& presentation = modelManager.load("presentation",groundVertices,"../textures/presentation1.png");
	 auto& eb408 = modelManager.load("eb408","../models/EB408.obj","../textures/T_EB408.png");
	 auto& chair = modelManager.load("chair","../models/OrangeChair.obj","../textures/T_OrangeChair_BaseColor.png");
	 auto& table = modelManager.load("table","../models/table.obj","../textures/T_Table_BaseColor.png");
	 auto& whiteboard = modelManager.load("whiteboard","../models/Whiteboard.obj","../textures/T_Whiteboard_BaseColor.png");
	 auto& door = modelManager.load("door","../models/Door.obj","../textures/T_door_basecolor.png");
	 auto& computer = modelManager.load("computer","../models/Computer.obj", "../textures/Computer.png");
	 auto& sun = modelManager.load("sun",sunVertices,9);
	 auto& moon = modelManager.load("moon",moonVertices,9);
	 auto& earth = modelManager.load("earth",earthVertices,9);
	 auto& login = modelManager.load("login",loginVertices,6);
	 auto& rocket = modelManager.load("rocket",rocketVertices,9); 
	 auto& sphere = modelManager.load("sphere",sphereVertices);
	 auto& trees = resources.imodels.load("trees",treeVertices);

	 std::string folderPath = "../textures/cubemaps/clouds_dark/";
	 std::vector<std::string> faces = { folderPath + "posx.png", folderPath + "negx.png", folderPath + "posy.png",folderPath + "negy.png",folderPath + "posz.png",folderPath + "negz.png"};
	 auto& skyTex = resources.cubemaps.load("sky", faces);

	 // Space example
	 m_scenes.push_back(new Scene());
	 // Forest example
	 m_scenes.push_back(new Scene());
	 // Classroom example
	 m_scenes.push_back(new Scene());

	 // initialising scenes, recalculating perespective, setting editor object
	glm::mat4 proj = glm::perspective(glm::radians(60.0f),
 		                               800.0f / 600.0f,
  		                               0.1f, 100.0f);
	for(auto& scene : m_scenes){
		scene->init();
		scene->getCamera()->setProjMatrix(proj);
		scene->setEditorObjModel(*modelManager.get("chair"));
	}
		
	//Helper composite function
	auto createTransform = [](glm::vec3 translation,glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f, glm::vec3 rotation = glm::vec3{0.0f,1.0f,0.0f}, float animSpeed = 0.0f) {
		auto t = std::make_shared<TransformComposite>();
		t->addNode(std::make_shared<Translation>(translation));
		t->addNode(std::make_shared<Rotation>(glm::radians(angle),rotation,animSpeed));
		t->addNode(std::make_shared<Scaling>(scale));
		return t;
	};	

	// **************************************************** Creating the space scene
	{ 
    m_scenes[0]->addObject("moon",moon,lightsShaderUntex);
	m_scenes[0]->addObject("earth",earth,lightsShaderUntex);
	m_scenes[0]->addObject("login",login,lightsShaderUntex);
	m_scenes[0]->addObject("rocket",rocket,lightsShaderUntex);
	m_scenes[0]->addLightObject("sun",sun,ambient, std::make_shared<DirLight>(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3( 0.05f, 0.05f, 0.05f), glm::vec3(0.4f), glm::vec3(0.5f)));

	lightsShaderUntex.setCamera(m_scenes[0]->getCamera()); 
	ambient.setCamera(m_scenes[0]->getCamera()); 

	//sun 
	auto sun = std::make_shared<TransformComposite>();
	sun->addNode(std::make_shared<Rotation>(glm::radians(70.f), glm::vec3(0.f, -1.f, 0.f), 0.3f));
	sun->addNode(std::make_shared<Scaling>(.3f));
	m_scenes[0]->getLightObject("sun").setTransform(sun);

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

	// *************************************************** Setting up forest scene
	m_scenes[1]->addObject("ground",ground,lightsShader);
	m_scenes[1]->addObject("trees",trees,treecolor);
	m_scenes[1]->addObject("shrek",shrek,lightsShader);
	m_scenes[1]->addObject("fiona",fiona,lightsShader); 
	m_scenes[1]->addSkybox("../models/skybox.obj",skyTex, skyShader);
	m_scenes[1]->addLightObject("moon", sphere, diffuse, std::make_shared<DirLight>(glm::vec3(-0.2, -1.0f, -0.3f), glm::vec3(0.03f, 0.05f, 0.12f), glm::vec3(0.015, 0.020f, 0.040f), glm::vec3(0.015f, 0.020f, 0.035f)));
	m_scenes[1]->addLightObject("flashlight", sphere, diffuse, std::make_shared<SpotLight>(glm::vec3(0.0f), glm::vec3(0.0f),glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1),
	1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f))));

	{ // instanced trees
	 std::vector<std::shared_ptr<TransformComposite>> composites;
	 for(int i = 0; i <= 73; i++){
	 float randomScale = randomFloat(0.6f, 1.00f); 
	 float randomZ = randomFloat(-35.0f, 35.0f);
	 float randomX = randomFloat(-35.0f, 35.0f);
	 float randomRot = randomFloat(-90.0f,90.0f);
	
	 composites.push_back(createTransform(glm::vec3(randomX, 0.0f, randomZ), glm::vec3(randomScale), randomRot));
	 }
	 trees.setInstances(composites);
	 m_scenes[1]->getObject("trees");
	 }

	 for(int i = 0; i < 4; i++){ // creating fireflies
		std::string name = "firefly" + std::to_string(i);
		m_scenes[1]->addLightObject(name.c_str(), sphere, ambient6, std::make_shared<PointLight>(glm::vec3(0.0f), glm::vec3(0.01f, 0.05f, 0.01f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.01f, 0.01f, 0.01f),
		1.0f, 0.09f, 0.032f));
		m_scenes[1]->getLightObject(name).setTransform(std::make_shared<Scaling>(0.01f));
	 	m_scenes[1]->getLightObject(name).startRoaming(glm::vec3(-25.0, 0.5, -25.0), glm::vec3(25.0, 3.0, 25.0), 1.5f);
	 }

	 m_scenes[1]->getLightObject("moon").setTransform(std::make_shared<Scaling>(0.f));
	 m_scenes[1]->getLightObject("flashlight").setTransform(std::make_shared<Scaling>(0.0f));
	 m_scenes[1]->getObject("shrek").setTransform(createTransform(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(-1,1,1)));
	 m_scenes[1]->getObject("fiona").setTransform(createTransform(glm::vec3(3.0f,0.0f,0.0f),glm::vec3(-1,1,1)));
	 


	// *************************************************** Setting classroom scene
	m_scenes[2]->addObject("presentation", presentation,textureShader);
	m_scenes[2]->addObject("classroom",eb408,lightsShader);
	m_scenes[2]->addObject("door",door,lightsShader);
	m_scenes[2]->addObject("whiteboard",whiteboard,phongTex);
	m_scenes[2]->addSkybox("../models/skybox.obj",skyTex, skyShader);
	m_scenes[2]->addLightObject("light1", sphere, diffuse, std::make_shared<PointLight>(glm::vec3(0.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f),
	1.0f, 0.09f, 0.032f));

	
	std::vector<glm::vec3> chairPositions = {{-0.74,0,1.17},{-0.74,0,2.17},{-0.74,0,3.17},{-0.74,0,4.17},{-0.74,0,5.17},{-0.74,0,5.9},
	 										 {0.82,0,1.17},{0.82,0,2.17},{0.82,0,3.17},{0.82,0,4.17},{0.82,0,5.17},{0.82,0,5.9},
											 {2.42,0,1.17},{2.42,0,2.17},{2.42,0,3.17},{2.42,0,4.17},{2.42,0,5.17},{2.42,0,5.9},
											 {-2.92,0,0.92}};
	std::vector<glm::vec3> tablePositions = {{-1,0,1.7}, {-1,0,3.5}, {-1,0,5.3}, {0.5,0,1.7}, {0.5,0,3.5}, {0.5,0,5.3}, {2.1,0,1.7},{2.1,0,3.5}, {2.1,0,5.3}, {-2.4,0,1.3}};
	std::vector<glm::vec3> computerPositions = {{-1.12,0.67,1.25},{-1.12,0.67,2.25},{-1.12,0.67,3.25},{-1.12,0.67,4.20},{-1.12,0.67,5.25},{-1.17,0.67,6.0},
												{0.48,0.67,1.25},{0.48,0.67,2.25},{0.48,0.67,3.25},{0.48,0.67,4.20},{0.48,0.67,5.25},{0.48,0.67,6.0},
												{2.1,0.67,1.25},{2.1,0.67,2.25},{2.1,0.67,3.25},{2.1,0.67,4.20},{2.1,0.67,5.25},{2.1,0.67,6.0},
												{-2.53,0.67,0.92}};

	for(int i = 0; i < 19; i++){
		std::string name = "chair" + std::to_string(i);
		m_scenes[2]->addObject(name,chair,lightsShader);
		m_scenes[2]->getObject(name).setTransform(createTransform(chairPositions.at(i),glm::vec3(1.0f), i == 18 ? 90.0f : -90.0f));
	}
	for(int i = 0; i < 10; i++){
		std::string name = "table" + std::to_string(i);
		m_scenes[2]->addObject(name,table,lightsShader);
		m_scenes[2]->getObject(name).setTransform(createTransform(tablePositions.at(i), glm::vec3(1.0f), 90.0f));
	}
	for(int i = 0; i < 19; i++){
		std::string name = "computer" + std::to_string(i);
		m_scenes[2]->addObject(name,computer,lightsShader);
		m_scenes[2]->getObject(name).setTransform(createTransform(computerPositions.at(i), glm::vec3(0.001f), i == 18 ? -90.0f : 90.0f));
	}	

	m_scenes[2]->getObject("whiteboard").setTransform(createTransform(glm::vec3(-3.8f,0.0f,4.0f)));
	m_scenes[2]->getObject("presentation").setTransform(createTransform(glm::vec3(-2.78f,0.985f,5.14f), glm::vec3(4.470f, 2.51f, 1.0f), -90.0f));
	m_scenes[2]->getObject("door").setTransform(createTransform(glm::vec3(-2.39f,0.0f,7.7f)));
	m_scenes[2]->getObject("classroom").setMaterial(Material::getMaterial("silver"));
	m_scenes[2]->getLightObject("light1").setTransform(createTransform(glm::vec3(-0.0761889, 3.05, 3.62653), glm::vec3(0.1)));
	
	main_scene = m_scenes[1];
}


void Application::run(){
	double lastTime = glfwGetTime();
	glEnable(GL_MULTISAMPLE);
	GLFWwindow* win = window.get();
	glEnable(GL_BLEND);


	double lastFrameTime = glfwGetTime();
	double lastFpsTime   = lastFrameTime;
	int frameCount = 0;

	while (!glfwWindowShouldClose(win))
	{
	// FPS
	double now = glfwGetTime();
	float deltaTime = static_cast<float>(now - lastFrameTime);  
	lastFrameTime = now;

	frameCount++;
	if (now - lastFpsTime >= 1.0) {
		double fps = frameCount / (now - lastFpsTime);
		std::string title = "ZPG - " + mode + " mode " + std::to_string(static_cast<int>(fps)) + " FPS";
		glfwSetWindowTitle(win, title.c_str());
		frameCount = 0;
		lastFpsTime = now;
	}

	// if in insert mode
	if(getMode() == 1){
		onPreviewObject(getWorldPosition());
	}else{
		onPreviewObject(glm::vec3(-10000.0f,-10000.0f,-10000.0f));
	}

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
	main_scene->update(deltaTime);

	glfwSwapBuffers(win);
	glfwPollEvents();
	}
}