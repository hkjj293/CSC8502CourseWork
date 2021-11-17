#include "hkjjgl/SceneManager.h"
#include "hkjjgl/Scene.h"

bool createScenes(SceneManager& sceneManager);

int main() {
	SceneManager sceneManager;
	if (!createScenes(sceneManager)) {
		return -1;
	}
	return sceneManager.Start(0);
}

bool createScenes(SceneManager& sceneManager) {
	Scene* scene_1 = new Scene();
	if (!scene_1->HasInitialised()) return false;
	sceneManager.AddScene(scene_1);
	return true;
}