#include "SceneManager.h"

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {
	for (auto it = this->scenes.begin(); it != this->scenes.end(); ++it) {
		delete (*it);
	}
	delete w;
	delete r;
}

void SceneManager::addScene(Scene* scene) {
	this->scenes.push_back(scene);
}

int SceneManager::startScene(int num) {

}

int SceneManager::startScene(int num) {

}