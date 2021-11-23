#pragma on
#include "Matrix4.h"
#include "Vector3.h"
#include "SceneNode.h"

class Camera : public SceneNode {
public:
	Camera(std::string name = "mainCamera") : SceneNode(name) {
		this->name = name;
		roll = 0.0f;
		yaw = 0.0f;
		pitch = 0.0f;
		position = Vector3(0, 0, 0);
		fov = 45.0f;
	}

	Camera(std::string name, float pitch, float yaw, float roll, float fov, Vector3 position) : SceneNode() {
		this->name = name;
		this->roll = roll;
		this->yaw = yaw;
		this->pitch = pitch;
		this->position = position;
		this->fov = fov;

	}

	~Camera() {}

	virtual void Update(float dt) override;

	Matrix4 BuildViewMatrix();

	float fov;
	float roll;
	float yaw;
	float pitch;
	Vector3 position;
};