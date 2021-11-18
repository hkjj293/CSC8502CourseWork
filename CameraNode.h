#pragma once

#include "hkjjgl/SceneNode.h"

class Camera;

class CameraNode : public SceneNode {
public:
	CameraNode(Camera* cam);
	~CameraNode();
	
	virtual void Update(float dt) override;
	Camera* GetCamera();

protected:
	Camera* cam;
};