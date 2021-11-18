#include "hkjjgl/SceneManager.h"
#include "Scene1.h"

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
	if (!scene_1->HasInitialised()) return false;
	sceneManager.AddScene(scene_1);
	return true;
}