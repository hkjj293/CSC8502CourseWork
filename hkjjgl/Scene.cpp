#include "Scene.h"

Scene::Scene() {
	init = false;
	forceQuit = false;
	nextScene = 0;
	//root = nullptr;
}

Scene::~Scene() {
	//delete root;
}

//SceneNode* Scene::GetRoot() {
//	return root;
//}

bool Scene::HasInitialised() const {
	return init;
}

bool Scene::IsEnd() const{
	return forceQuit;
}

int Scene::Next() const{
	return nextScene;
}