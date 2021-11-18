#pragma on
#include "Matrix4.h"
#include "Vector3.h"

class Camera {
public:
	Camera() {
		roll = 0.0f;
		yaw = 0.0f;
		pitch = 0.0f;
		fov = 45.0f;
		forward = Vector3(0, 0, -1);
		right = Vector3(1, 0, 0);
	}

	Camera(float pitch, float yaw, float roll, float fov, Vector3 position) {
		this->roll = roll;
		this->yaw = yaw;
		this->pitch = pitch;
		this->position = position;
		this->fov = fov;
		forward = Vector3(0, 0, -1);
		right = Vector3(1, 0, 0);
	}

	~Camera() {}

	void UpdateCamera(float dt = 1.0f);

	Matrix4 BuildViewMatrix();

	float fov;
	float roll;
	float yaw;
	float pitch;
	Vector3 position;
	Vector3  forward;
	Vector3  right;
};