#pragma once

//class SceneNode;

class Scene {
public:
	Scene();
	virtual ~Scene();

	//SceneNode* GetRoot();

	virtual void Update() {};

	bool HasInitialised() const;
	bool IsEnd() const;
	int Next() const;

protected:
	//SceneNode* root;

	int nextScene;
	bool forceQuit;
	bool init;
};