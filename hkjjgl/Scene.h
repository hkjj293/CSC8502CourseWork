#pragma once
#include <string>

class SceneNode;
class ResourceManager;
class Camera;

class Scene {
public:
	Scene(std::string name);
	virtual ~Scene();

	SceneNode* GetRoot();
	ResourceManager* GetResourceManager();
	std::string GetName();

	void SetName(std::string name);

	virtual void Update() = 0;
	virtual bool Load() = 0;
	virtual bool Unload() = 0;

	bool HasInitialised() const;
	bool IsEnd() const;
	std::string Next() const;

protected:
	SceneNode* root;
	ResourceManager* rManager;
	Camera* mainCamera;

	std::string name;

	std::string nextScene;
	bool forceQuit;
	bool init;
};