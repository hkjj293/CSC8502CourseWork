#include "hkjjgl/SceneManager.h"
#include "Scene1.h"
#include <iostream>

bool createScenes(SceneManager& sceneManager);

int main() {
	SceneManager sceneManager;
	if (!createScenes(sceneManager)) {
		return -1;
	}
	return sceneManager.Start();
}

bool createScenes(SceneManager& sceneManager) {
	Scene1* scene_1 = new Scene1();
	std::cout << scene_1->GetName() << std::endl;
	if (!scene_1->HasInitialised()) 
		return false;
	sceneManager.AddScene(scene_1);
	sceneManager.Start("scene1");
	return true;
}