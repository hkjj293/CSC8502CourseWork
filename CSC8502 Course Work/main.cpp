#include "../hkjjgl/SceneManager.h"

bool createScenes(SceneManager& sceneManager);

int main() {
	SceneManager sceneManager;
	if (!createScenes(sceneManager)) {
		return -1;
	}
	return sceneManager.startScene(0);
}

bool createScenes(SceneManager& sceneManager) {
	Scene* scene1 = new Scene();
	sceneManager.addScene(scene1);
	sceneManager.refresh();
}