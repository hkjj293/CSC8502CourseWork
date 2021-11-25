#pragma once
#include <vector>
#include <string>
#include <map>
#include <chrono>

class Scene;
class Renderer;
class Window;
class GameTimer;

class SceneManager {
public:
	SceneManager(std::string title = "OpenGL Framework", int sizeX = 800, int sizeY = 600, bool fullScreen = false);
	~SceneManager();

	void AddScene(Scene* scene);
	void DeleteScene(std::string name);

	virtual int Start(std::string name = "main");

	float GetFrameRate();

	bool HasInitialised() const;

protected:
	bool hasScene(std::string name);

	bool init;

	GameTimer* t;
	Window* w;
	Renderer* r;
	std::map<std::string,Scene*> scenes;
};