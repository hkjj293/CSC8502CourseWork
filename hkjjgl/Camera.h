#pragma on
#include "Matrix4.h"
#include "Vector3.h"
#include "SceneNode.h"

class Camera : public SceneNode {
public:
	Camera(std::string name = "mainCamera") : SceneNode(name) {
		roll = 0.0f;
		yaw = 0.0f;
		pitch = 0.0f;
		position = Vector3(0, 0, 0);
		fov = 45.0f;
		time = 0;
	}

	Camera(std::string name, float pitch, float yaw, float roll, float fov, Vector3 position) : SceneNode(name) {
		this->roll = roll;
		this->yaw = yaw;
		this->pitch = pitch;
		this->position = position;
		this->fov = fov;
		time = 0;
	}

	~Camera() {}

	virtual void Update(float dt) override;

	Matrix4 BuildViewMatrix();
	Matrix4 BuildTransformMatrix();
	Matrix4 BuildTranslationMatrix();
	Matrix4 BuildRotationMatrix();
	Matrix4 BuildProjMatrix(float width, float height);

	float fov;
	float roll;
	float yaw;
	float pitch;
	float time;
	Vector3 position;
};