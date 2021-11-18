#pragma once
#include "hkjjgl/Scene.h"

class Scene1 : public Scene {
public:
	Scene1();
	virtual ~Scene1() override;

	virtual void Update() override;
	virtual bool Load() override;
	virtual bool Unload() override;

protected:
};