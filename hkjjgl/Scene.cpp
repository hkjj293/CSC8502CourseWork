#include "Scene.h"
#include "SceneNode.h"
#include "ResourceManager.h"

Scene::Scene(std::string name) {
	init = false;
	forceQuit = false;
	this->name = name;
	nextScene = name;
	root = nullptr;
	rManager = nullptr;
	timer = new GameTimer();
	init = true; 
}

Scene::~Scene() {
	if (this->root)
		delete this->root;
	root = nullptr;

	if (this->rManager)
		delete this->rManager;
	rManager = nullptr;
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