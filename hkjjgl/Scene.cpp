#include "Scene.h"
#include "SceneNode.h"
#include "ResourceManager.h"

Scene::Scene(std::string name) {
	init = false;
	forceQuit = false;
	this->name = name;
	nextScene = name;
	root = nullptr;
	timer = new GameTimer();
	// // debug code only!!! Comment it when release or not used
	//init = true; 
}

Scene::~Scene() {
	//if(root) 
		//delete root;

	//if(rManager) 
		//delete rManager;

	//if(mainCamera) 
		//delete mainCamera;
}

SceneNode* Scene::GetRoot() {
	return root;
}

ResourceManager* Scene::GetResourceManager() {
	return rManager;
}

std::string Scene::GetName() {
	return name;
}

void Scene::SetName(std::string name) {
	this->name = name;
}

bool Scene::HasInitialised() const {
	return init;
}

bool Scene::IsEnd() const{
	return forceQuit;
}

std::string Scene::Next() const{
	return nextScene;
}