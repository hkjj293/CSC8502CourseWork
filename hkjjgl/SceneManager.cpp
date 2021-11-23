#include "SceneManager.h"
#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "GameTimer.h"


SceneManager::SceneManager() {
	init = false;

	w = new Window("Deferred Rendering!", 1280, 720, false);
	if (!w->HasInitialised()) {
		return;
	}

	r = new Renderer(*w);
	if (!r->HasInitialised()) {
		return;
	}

	w->LockMouseToWindow(true);
	w->ShowOSPointer(false);

	init = true;
}

SceneManager::~SceneManager() {
	for (auto it = scenes.begin(); it != scenes.end(); ++it) {
		delete it->second;
	}
	delete w;
	//delete r;
}

void SceneManager::AddScene(Scene* scene) {
	scenes[scene->GetName()] = scene;
}

void SceneManager::DeleteScene(std::string sceneName) {
	if (!hasScene(sceneName)) return;
	delete scenes[sceneName];
	scenes.erase(sceneName);
}

int SceneManager::Start(std::string sceneName) {
	if (!hasScene(sceneName)) return -1;
	if (!scenes[sceneName]->Load()) return -2;

	while (w->UpdateWindow() && !scenes[sceneName]->IsEnd() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		scenes[sceneName]->Update();
		r->Render(scenes[sceneName]);

		if (scenes[sceneName]->Next() != sceneName) {
			if (!hasScene(scenes[sceneName]->Next())) return -1;
			sceneName = scenes[sceneName]->Next();

			if(!scenes[sceneName]->Load()) return -2;
			//r->LoadSceneResources(scenes[sceneName]);
		}	
	}
	return 0;
}

float SceneManager::GetFrameRate() {
	return 1 / t->GetTimeDeltaSeconds();
}

bool SceneManager::HasInitialised() const{
	return init;
}

bool SceneManager::hasScene(std::string sceneName) {
	auto it = scenes.find(sceneName);
	return !(it == scenes.end());
}