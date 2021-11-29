#include "Camera.h"
#include "Window.h"
#include <algorithm>

void  Camera::Update(float dt) {
	SceneNode::Update(dt);
	position = Vector3(worldTransform.values[12], worldTransform.values[13], worldTransform.values[14]);
	/*if (!Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
		position.y -= speed;
		if (position.y < 0) position.y = 0;
	}*/;
}

Matrix4 Camera::BuildViewMatrix() {		// All operations must be invert
	return Matrix4::Rotation(-roll, Vector3(0, 0, -1)) *
		Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
}

Matrix4 Camera::BuildTransformMatrix() {		// All operations must be invert
	return Matrix4::Translation(position) *
		Matrix4::Rotation(yaw, Vector3(0, 1, 0)) *
		Matrix4::Rotation(pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(roll, Vector3(0, 0, -1));
}

Matrix4 Camera::BuildTranslationMatrix() {
	return Matrix4::Translation(position);
}

Matrix4 Camera::BuildRotationMatrix() {
	return Matrix4::Rotation(yaw, Vector3(0, 1, 0))*
		Matrix4::Rotation(pitch, Vector3(1, 0, 0))*
		Matrix4::Rotation(roll, Vector3(0, 0, -1));
}

Matrix4 Camera::BuildProjMatrix(float width, float height) {		// All operations must be invert
	return Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, fov);
}