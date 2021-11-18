#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"
#include <vector>

class SceneNode {
public:
	SceneNode(std::string name, Mesh * m = NULL, Vector4  colour = Vector4(1, 1, 1, 1));
	~SceneNode();

	virtual void Update(float dt);

	void AddChild(SceneNode* s);
	void RemoveChild(std::string name);

	Vector4 GetColour() { return colour; }
	Vector3 GetModelScale() { return modelScale; }

	Matrix4 GetWorldTransform() { return worldTransform; }
	Matrix4 GetLocalTransform() { return transform; }

	static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera < b->distanceFromCamera) ? true : false;
	}

protected:
	std::string name;

	SceneNode* parent;
	Mesh* mesh;

	Matrix4 worldTransform;
	Matrix4 transform;

	Vector3 modelScale;
	Vector4 colour;

	std::vector<SceneNode*> children;

	float boundingRadius;
	float distanceFromCamera;
	GLuint texture;
};