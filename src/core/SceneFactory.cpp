#include "SceneFactory.h"
#include "../models/earth.h"
#include "../models/moon.h"
#include "../models/sun.h"
#include "../models/login_points.h"
#include "../models/rocket.h"
#include "../models/sphere.h"
#include "../models/tree.h"
#include "../models/clock.h"
#include "Logger.hpp"
#include "transformation/Scaling.h"
#include "transformation/Translation.h"
#include "transformation/Rotation.h"
#include "transformation/TransformComposite.h"
#include "graphics/Model.h"
#include "graphics/InstancedModel.h"
#include "graphics/Cubemap.h"
#include "graphics/Skybox.h"
#include "graphics/lights/SpotLight.h"
#include "core/Camera.h"

float SceneFactory::randomFloat(float lo, float hi) {
     static std::mt19937 gen{std::random_device{}()};
     std::uniform_real_distribution<float> dist(lo, hi);
     return dist(gen);
}

Scene& SceneFactory::space(Resources* resources){
    Scene* scene = new Scene();
    if(!scene)
        Logger::Error("Error creating scene!");
	std::vector<float> earthVertices(earth, earth+ (sizeof(earth) / sizeof(float)));
    std::vector<float> sunVertices(sun,sun + (sizeof(sun) / sizeof(float)));
    std::vector<float> moonVertices(moon,moon + (sizeof(moon) / sizeof(float)));
	std::vector<float> loginVertices(login,login + (sizeof(login) / sizeof(float)));
	std::vector<float> rocketVertices(rocket, rocket + (sizeof(rocket) / sizeof(float)));

	auto& shaderManager = resources->shaders;
	auto& ambient = shaderManager.load("ambient", "shaders/lighting_9float.vert", "shaders/ambient.frag");
	auto& lightsUntex = shaderManager.load("lightsUntex", "shaders/lighting_9float.vert","shaders/phong_ubo.frag");

	auto& modelManager = resources->models;
	auto& sun = modelManager.load("sun",sunVertices,9);
	auto& moon = modelManager.load("moon",moonVertices,9);
	auto& earth = modelManager.load("earth",earthVertices,9);
	auto& login = modelManager.load("login",loginVertices,6);
	auto& rocket = modelManager.load("rocket",rocketVertices,9); 

    scene->addObject("moon",moon,lightsUntex);
	scene->addObject("earth",earth,lightsUntex);
	scene->addObject("login",login,lightsUntex);
	scene->addObject("rocket",rocket,lightsUntex);
	scene->addLightObject("sun",sun,ambient, std::make_shared<DirLight>(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3( 0.05f, 0.05f, 0.05f), glm::vec3(0.4f), glm::vec3(0.5f)));

	lightsUntex.setCamera(scene->getCamera()); 
	ambient.setCamera(scene->getCamera()); 

    {
	//sun 
	auto sun = std::make_shared<TransformComposite>();
	sun->addNode(std::make_shared<Rotation>(glm::radians(70.f), glm::vec3(0.f, -1.f, 0.f), 0.3f));
	sun->addNode(std::make_shared<Scaling>(.3f));
	scene->getLightObject("sun").setTransform(sun);

	// earth orbit around sun
	auto earthOrbit = std::make_shared<TransformComposite>();
	earthOrbit->addNode(std::make_shared<Rotation>(0.f, glm::vec3(0.f, 1.f, 0.f), 0.5f));
	earthOrbit->addNode(std::make_shared<Translation>(glm::vec3(0.9f, 0.f, 0.f)));

	//  earth * scale
	auto earth = std::make_shared<TransformComposite>();
	earth->addNode(earthOrbit);
	earth->addNode(std::make_shared<Rotation>(glm::radians(70.f), glm::vec3(0.f, 1.f, 0.f), 8.0f));
	earth->addNode(std::make_shared<Scaling>(.03f));
	scene->getObject("earth").setTransform(earth);

	// moon = earth orbit * orbit around earth * scale 
	auto moon = std::make_shared<TransformComposite>();
	moon->addNode(earthOrbit);                                                          
	moon->addNode(std::make_shared<Rotation>(0.f, glm::vec3(0.f, 1.f, 0.f), 2.f));        
	moon->addNode(std::make_shared<Translation>(glm::vec3(0.2f, 0.f, 0.f)));              
	moon->addNode(std::make_shared<Scaling>(.01f));
	scene->getObject("moon").setTransform(moon);

	auto login = std::make_shared<TransformComposite>();
	login->addNode(std::make_shared<Translation>(glm::vec3(1.0f, 1.0f, 1.0f)));              
	login->addNode(std::make_shared<Rotation>(0.f, glm::vec3(0.f, 1.f, 0.f), 2.f));        
	login->addNode(std::make_shared<Scaling>(.25f));
	scene->getObject("login").setTransform(login);

	// earth orbit around sun
	auto shipOrbit = std::make_shared<TransformComposite>();
	shipOrbit->addNode(std::make_shared<Rotation>(0.f, glm::vec3(0.f, -1.f, 0.f), 0.5f));
	shipOrbit->addNode(std::make_shared<Translation>(glm::vec3(1.9f, 0.f, 0.f)));

	auto rocket = std::make_shared<TransformComposite>();
	rocket->addNode(shipOrbit);
	rocket->addNode(std::make_shared<Rotation>(0.f, glm::vec3(0.f, 0.f, 1.f),1.0f));        
	rocket->addNode(std::make_shared<Scaling>(.00009f));
	scene->getObject("rocket").setTransform(rocket);
    }
    return *scene;
}


Scene& SceneFactory::forest(Resources* resources){
    Scene* scene = new Scene();
    if(!scene)
        Logger::Error("Error creating scene!");
    return *scene;
}

Scene& SceneFactory::forestNight(Resources* resources){
    Scene* scene = new Scene();
    if(!scene)
        Logger::Error("Error creating scene!");

	std::vector<float> sphereVertices(sphere, sphere+ (sizeof(sphere) / sizeof(float)));
	std::vector<float> treeVertices(tree, tree+ (sizeof(tree) / sizeof(float)));

    auto& shaderManager = resources->shaders;
	auto& lightsShader = shaderManager.load("lights", "shaders/lighting_texture.vert","shaders/phong_texture_ubo.frag");
	auto& treecolor = shaderManager.load("treecolor","shaders/treeColors.vert","shaders/phong_ubo.frag");
	auto& skyShader = resources->shaders.load("skybox", "shaders/skybox.vert", "shaders/skybox.frag");
    auto& diffuse = shaderManager.load("diffuse","shaders/lighting_6float.vert","shaders/diffuse.frag");
    auto& ambient6 = shaderManager.load("ambient6","shaders/lighting_6float.vert","shaders/ambient.frag");

	auto& modelManager = resources->models;
	auto& shrek = modelManager.load("shrek","../models/shrek.obj","../textures/shrek.png");
	auto& fiona = modelManager.load("fiona","../models/fiona.obj","../textures/fiona.png");
	auto& ground = modelManager.load("ground","../models/teren.obj","../textures/grass.jpg");
    auto& sphere = modelManager.load("sphere",sphereVertices);
	auto& trees = resources->imodels.load("trees",treeVertices);

   	auto createTransform = [](glm::vec3 translation,glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f, glm::vec3 rotation = glm::vec3{0.0f,1.0f,0.0f}, float animSpeed = 0.0f) {
		auto t = std::make_shared<TransformComposite>();
		t->addNode(std::make_shared<Translation>(translation));
		t->addNode(std::make_shared<Rotation>(glm::radians(angle),rotation,animSpeed));
		t->addNode(std::make_shared<Scaling>(scale));
		return t;
	};	

    std::string folderPath = "../textures/cubemaps/clouds_dark/";
	std::vector<std::string> faces = { folderPath + "posx.png", folderPath + "negx.png", folderPath + "posy.png",folderPath + "negy.png",folderPath + "posz.png",folderPath + "negz.png"};
	auto& skyTex = resources->cubemaps.load("sky", faces);

	scene->addObject("ground",ground,lightsShader);
	scene->addObject("trees",trees,treecolor);
	scene->addObject("shrek",shrek,lightsShader);
	scene->addObject("fiona",fiona,lightsShader); 
	scene->addSkybox("../models/skybox.obj",skyTex, skyShader);
	scene->addLightObject("moon", sphere, diffuse, std::make_shared<DirLight>(glm::vec3(-0.2, -1.0f, -0.3f), glm::vec3(0.03f, 0.05f, 0.012f), glm::vec3(0.015, 0.020f, 0.040f), glm::vec3(0.015f, 0.020f, 0.035f)));
	scene->addLightObject("flashlight", sphere, diffuse, std::make_shared<SpotLight>(glm::vec3(0.0f), glm::vec3(0.0f),glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1),
	1.0f, 0.09f, 0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f))));

	// instanced trees
	std::vector<std::shared_ptr<TransformComposite>> composites;
	for(int i = 0; i <= 100; i++){
		float randomScale = randomFloat(0.6f, 1.00f); 
		float randomZ = randomFloat(-35.0f, 35.0f);
		float randomX = randomFloat(-35.0f, 35.0f);
		float randomRot = randomFloat(-90.0f,90.0f);
		
		composites.push_back(createTransform(glm::vec3(randomX, 0.0f, randomZ), glm::vec3(randomScale), randomRot));
	}
		trees.setInstances(composites);
		scene->getObject("trees");

    // fireflies
    for(int i = 0; i < 4; i++){ 
		std::string name = "firefly" + std::to_string(i);
		scene->addLightObject(name.c_str(), sphere, ambient6, std::make_shared<PointLight>(glm::vec3(0.0f), glm::vec3(0.01f, 0.05f, 0.01f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.01f, 0.01f, 0.01f),
		1.0f, 0.09f, 0.032f));
		scene->getLightObject(name).setTransform(std::make_shared<Scaling>(0.01f));
	 	scene->getLightObject(name).startRoaming(glm::vec3(-25.0, 0.5, -25.0), glm::vec3(25.0, 3.0, 25.0), 1.5f);
	 }

	 scene->getLightObject("moon").setTransform(std::make_shared<Scaling>(0.f));
	 scene->getLightObject("flashlight").setTransform(std::make_shared<Scaling>(0.0f));
	 scene->getObject("shrek").setTransform(createTransform(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(-1,1,1)));
	 scene->getObject("fiona").setTransform(createTransform(glm::vec3(3.0f,0.0f,0.0f),glm::vec3(-1,1,1)));

     return *scene;
}
Scene& SceneFactory::classroom(Resources* resources){
    Scene* scene = new Scene();
    if(!scene)
        Logger::Error("Error creating scene!");
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

	std::vector<float> sphereVertices(sphere, sphere+ (sizeof(sphere) / sizeof(float)));
    std::vector<float> loginVertices(login,login + (sizeof(login) / sizeof(float)));
    std::vector<float> clockVertices(clockModel,clockModel+ (sizeof(clockModel) / sizeof(float)));

    auto& shaderManager = resources->shaders;
    auto& textureShader = shaderManager.load("texture","shaders/texture.vert","shaders/texture.frag");
	auto& lightsShader = shaderManager.load("lights", "shaders/lighting_texture.vert","shaders/phong_texture_ubo.frag");
	auto& skyShader = resources->shaders.load("skybox", "shaders/skybox.vert", "shaders/skybox.frag");
    auto& diffuse = shaderManager.load("diffuse","shaders/lighting_6float.vert","shaders/diffuse.frag");
    auto& phongTex = shaderManager.load("phongTex",  "shaders/lighting_texture.vert",  "shaders/phong_textures.frag");
	auto& clockShader = shaderManager.load("clock", "shaders/clock.vert", "shaders/phong_ubo.frag");

	auto& modelManager = resources->models;
	auto& presentation = modelManager.load("presentation",groundVertices,"../textures/presentation1.png");
	auto& eb408 = modelManager.load("eb408","../models/EB408.obj","../textures/T_EB408.png");
	auto& chair = modelManager.load("chair","../models/OrangeChair.obj","../textures/T_OrangeChair_BaseColor.png");
	auto& table = modelManager.load("table","../models/table.obj","../textures/T_Table_BaseColor.png");
	auto& whiteboard = modelManager.load("whiteboard","../models/Whiteboard.obj","../textures/T_Whiteboard_BaseColor.png");
	auto& door = modelManager.load("door","../models/Door.obj","../textures/T_door_basecolor.png");
	auto& computer = modelManager.load("computer","../models/Computer.obj", "../textures/Computer.png");
    auto& login = modelManager.load("login",loginVertices,6);
    auto& sphere = modelManager.load("sphere",sphereVertices);
	auto& clock = modelManager.load("clock",clockVertices, 9);

    auto createTransform = [](glm::vec3 translation,glm::vec3 scale = glm::vec3(1.0f), float angle = 0.0f, glm::vec3 rotation = glm::vec3{0.0f,1.0f,0.0f}, float animSpeed = 0.0f) {
		auto t = std::make_shared<TransformComposite>();
		t->addNode(std::make_shared<Translation>(translation));
		t->addNode(std::make_shared<Rotation>(glm::radians(angle),rotation,animSpeed));
		t->addNode(std::make_shared<Scaling>(scale));
		return t;
	};	 

    std::string folderPath = "../textures/cubemaps/clouds_dark/";
	std::vector<std::string> faces = { folderPath + "posx.png", folderPath + "negx.png", folderPath + "posy.png",folderPath + "negy.png",folderPath + "posz.png",folderPath + "negz.png"};
	auto& skyTex = resources->cubemaps.load("sky", faces);

	scene->addObject("presentation", presentation,textureShader);
	scene->addObject("classroom",eb408,lightsShader);
	scene->addObject("door",door,lightsShader);
	scene->addObject("whiteboard",whiteboard,phongTex);
	scene->addObject("clock", clock, clockShader);
	scene->addSkybox("../models/skybox.obj",skyTex, skyShader);
	scene->addLightObject("light1", sphere, diffuse, std::make_shared<PointLight>(glm::vec3(0.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f),
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
		scene->addObject(name,chair,lightsShader);
		scene->getObject(name).setTransform(createTransform(chairPositions.at(i),glm::vec3(1.0f), i == 18 ? 90.0f : -90.0f));
	}
	for(int i = 0; i < 10; i++){
		std::string name = "table" + std::to_string(i);
		scene->addObject(name,table,lightsShader);
		scene->getObject(name).setTransform(createTransform(tablePositions.at(i), glm::vec3(1.0f), 90.0f));
	}
	for(int i = 0; i < 19; i++){
		std::string name = "computer" + std::to_string(i);
		scene->addObject(name,computer,lightsShader);
		scene->getObject(name).setTransform(createTransform(computerPositions.at(i), glm::vec3(0.001f), i == 18 ? -90.0f : 90.0f));
	}	

	scene->getObject("whiteboard").setTransform(createTransform(glm::vec3(-3.8f,0.0f,4.0f)));
	scene->getObject("presentation").setTransform(createTransform(glm::vec3(-2.78f,0.985f,5.14f), glm::vec3(4.470f, 2.51f, 1.0f), -90.0f));
	scene->getObject("door").setTransform(createTransform(glm::vec3(-2.39f,0.0f,7.7f)));
	scene->getObject("clock").setTransform(createTransform(glm::vec3(-1.48f,2.36f,7.49f),glm::vec3(-0.25,0.25,0.25)));
	scene->getObject("classroom").setMaterial(Material::getMaterial("silver"));
	scene->getLightObject("light1").setTransform(createTransform(glm::vec3(-0.0761889, 3.05, 3.62653), glm::vec3(0.1)));

    return *scene;
}
Scene& SceneFactory::lightingShowcase(Resources* resources){
    Scene* scene = new Scene();
    if(!scene)
       Logger::Error("Error creating scene!");
    return *scene;
}
Scene& SceneFactory::triagle(Resources* resources){
    Scene* scene = new Scene();
    if(!scene)
       Logger::Error("Error creating scene!");
    return *scene;
}