#include "CameraNode.h"

CameraNode::CameraNode(Camera* cam) : SceneNode() {
	this->cam = cam;
}

CameraNode::~CameraNode() {
	if (cam)
		delete cam;
	cam = nullptr;
}

Camera* CameraNode::GetCamera() {
	return cam;
}