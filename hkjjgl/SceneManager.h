#pragma once
#include <vector>

class Scene;
class Renderer;
class Window;

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void addScene(Scene* scene);

	int startScene(int num);
	int startScene(Scene scene);
protected:
	std::vector<Scene*> scenes;
	Window* w;
	Renderer* r;
};