#pragma once
#include <vector>

class Scene;
//class Renderer;
class Window;

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void AddScene(Scene* scene);
	void DeleteScene(int sceneNum);

	virtual int Start(int num);

	bool HasInitialised() const;

protected:
	bool hasScene(int sceneNum);

	bool init;

	Window* w;
	//Renderer* r;
	std::vector<Scene*> scenes;
};