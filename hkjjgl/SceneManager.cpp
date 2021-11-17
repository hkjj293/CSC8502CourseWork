#include "SceneManager.h"
#include "Window.h"
//#include "Renderer.h"
#include "Scene.h"


SceneManager::SceneManager() {
	init = false;

	w = new Window("Deferred Rendering!", 1280, 720, false);
	if (!w->HasInitialised()) {
		return;
	}

	//r = new Renderer(*w);
	//if (!renderer.HasInitialised()) {
	//	return;
	//}

	w->LockMouseToWindow(true);
	w->ShowOSPointer(false);

	init = true;
}

SceneManager::~SceneManager() {
	for (auto it = this->scenes.begin(); it != this->scenes.end(); ++it) {
		delete (*it);
	}
	delete w;
	//delete r;
}

void SceneManager::AddScene(Scene* scene) {
	this->scenes.push_back(scene);
}

void SceneManager::DeleteScene(int sceneNum) {
	if (!hasScene(sceneNum)) return;
	delete scenes[sceneNum];
	this->scenes.erase(scenes.begin() + sceneNum);
}

int SceneManager::Start(int sceneNum) {
	if (!hasScene(sceneNum)) return -1;
	while (w->UpdateWindow() && !scenes[sceneNum]->IsEnd() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		scenes[sceneNum]->Update();
		//r->Render(scenes[sceneNum]->GetRoot());
		sceneNum = scenes[sceneNum]->Next();
	}
	return 0;
}

bool SceneManager::HasInitialised() const{
	return init;
}

bool SceneManager::hasScene(int sceneNum) {
	if (sceneNum >= scenes.size() || sceneNum < 0) {
		return false;
	}
	return true;
}