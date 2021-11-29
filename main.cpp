#include "hkjjgl/SceneManager.h"
#include "Scene1.h"
#include  <iostream>
#include  <algorithm>

bool createScenes(SceneManager& sceneManager);

int main() {
	SceneManager sceneManager("Course Work", std::min(GetSystemMetrics(SM_CXSCREEN),1920), std::min(GetSystemMetrics(SM_CYSCREEN),1080),false);
	if (!createScenes(sceneManager)) {
		return -1;
	}
	return sceneManager.Start("scene1");
}

bool createScenes(SceneManager& sceneManager) {
	Scene1* scene_1 = new Scene1();
	std::cout << scene_1->GetName() << std::endl;
	if (!scene_1->HasInitialised()) 
		return false;
	sceneManager.AddScene(scene_1);
	return true;
}