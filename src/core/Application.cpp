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

#include "core/Camera.h"
#include "Logger.hpp"
#include "SceneFactory.h"


Application::Application() :  window(1366,768,"ZPG") {}

Application::~Application(){
	for(auto& scene: m_scenes)
		delete(scene);
}

int Application::init(){
	Logger::Init();

	GLFWwindow* win = window.get();
	glfwSetWindowUserPointer(win, this);
	glfwSetMouseButtonCallback(win, Application::mouseButtonCallback);
	glfwSetCursorPosCallback(win, Application::cursorPosCallback);
	glfwSetFramebufferSizeCallback(win, Application::framebufferSizeCallback);

	// Initialize GLAD and load OpenGL function pointers
	if (!gladLoadGL(glfwGetProcAddress)) {
		 throw std::runtime_error("Failed to initialize GLAD");
	}

	glfwSetKeyCallback(win, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
		auto* app = static_cast<Application*>(glfwGetWindowUserPointer(w));
		if (app) app->m_keyboard.onKey(key, action);
	});

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

int Application::keybinds(){
	int numerals[9] = {GLFW_KEY_1,GLFW_KEY_2,GLFW_KEY_3,GLFW_KEY_4,GLFW_KEY_5,GLFW_KEY_6,GLFW_KEY_7,GLFW_KEY_8,GLFW_KEY_9} ;
	for(int i = 0; i < (m_scenes.size() % 9); i++){
		m_keyboard.bind(numerals[i], [this, i] {main_scene = m_scenes[i];} );
	}	

	m_keyboard.bind(GLFW_KEY_F, [this] {main_scene->getLightObject("flashlight").enable();});
	//m_keyboard.bind(GLFW_KEY_LEFT, [this] {main_scene->setEditorObjRotation(glm::radians(10.0f));});
	//m_keyboard.bind(GLFW_KEY_TAB, [this] {main_scene->switchMode();});
	/*
	if (glfwGetKey(win, GLFW_KEY_UP)  == GLFW_PRESS && !arrows[0]) {
			if(++modInx > resources.models.size()-1) modInx = 0;
    		main_scene->setEditorObjModel(resources.models.at(modInx));
			arrows[0] = true;
	}
	if (glfwGetKey(win, GLFW_KEY_DOWN)  == GLFW_PRESS && !arrows[1]) {
			if(modInx < 0) modInx = resources.models.size()-1;
    		main_scene->setEditorObjModel(resources.models.at(modInx--));
			arrows[1] = true;
	}*/
	return 0;
}

void Application::addScenes(){
	glm::mat4 proj = glm::perspective(glm::radians(60.0f),
 		                               800.0f / 600.0f,
  		                               0.1f, 100.0f);
	m_scenes.push_back(&SceneFactory::forestNight(&resources));
	m_scenes.push_back(&SceneFactory::space(&resources));
	m_scenes.push_back(&SceneFactory::classroom(&resources));

	for(auto& scene : m_scenes){
		scene->init();
		scene->getCamera()->setProjMatrix(proj);
		//scene->setEditorObjModel(*modelManager.get("chair"));
	}
	main_scene = m_scenes[0];
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
		glReadPixels(x, height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &id);

		// selection range only from 1-255
		Logger::Debug("Clicked on object: " ,id); 
		app->onClickSelectObject(id);
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

void Application::onPreviewObject(glm::vec3 position){
	main_scene->setEditorObjTranslation(position);
}

void Application::onClickSelectObject(GLuint id){
	main_scene->setId(id);
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
		std::string title = "ZPG - " + std::to_string(static_cast<int>(fps)) + " FPS";
		glfwSetWindowTitle(win, title.c_str());
		frameCount = 0;
		lastFpsTime = now;
		std::string dc = "Rendercalls: " + std::to_string(static_cast<int>(RenderStats::drawCalls));
	}

	// Movement camera
	glm::vec3 dir(0.0f);
	dir.x += m_keyboard.isDown(GLFW_KEY_W) - m_keyboard.isDown(GLFW_KEY_S);
	dir.y += m_keyboard.isDown(GLFW_KEY_D) - m_keyboard.isDown(GLFW_KEY_A);
	main_scene->getCamera()->setMovement(dir * 0.15f);

	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	main_scene->getCamera()->update(1.0f);

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