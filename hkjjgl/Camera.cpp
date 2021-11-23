#include "Camera.h"
#include "Window.h"
#include <algorithm>

void  Camera::Update(float dt) {
	float  speed = 100.0f * dt;
	pitch -= (Window::GetMouse()->GetRelativePosition().y)*3.0f;
	yaw -= (Window::GetMouse()->GetRelativePosition().x)*3.0f;
	fov -= (Window::GetMouse()->GetWheelMovement())*3.0f;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_Q)) {
		roll -= speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_E)) {
		roll += speed;
	}

	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	if (roll < 0) {
		roll += 360.0f;
	}
	if (roll > 360.0f) {
		roll -= 360.0f;
	}

	Matrix4  rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0)) *
		Matrix4::Rotation(pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(roll, Vector3(0, 0, -1));
	Vector3  forward = rotation * Vector3(0, 0, -1);
	Vector3  right = rotation * Vector3(1, 0, 0);

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
		position += forward * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
		position -= forward * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		position -= right * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		position += right * speed;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
		position.y += speed;
	}
	/*if (!Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
		position.y -= speed;
		if (position.y < 0) position.y = 0;
	}*/
}

Matrix4 Camera::BuildViewMatrix() {		// All operations must be invert
	return Matrix4::Rotation(-roll, Vector3(0, 0, -1)) *
		Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
}