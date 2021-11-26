#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"
#include <map>

class Material;

class SceneNode {
public:
	SceneNode() {};
	SceneNode(std::string name, Mesh * m = NULL, Vector4  colour = Vector4(1, 1, 1, 1));
	~SceneNode();

	virtual void Update(float dt);
	virtual void Draw();

	void AddChild(SceneNode* s);
	void RemoveChild(std::string name);
	SceneNode* FindChild(std::string name);
	SceneNode* GetChild(int index);
	std::map<std::string, SceneNode*>::iterator GetFirstChild();
	std::map<std::string, SceneNode*>::iterator GetChildrenEnd();
	int GetChildSize();

	SceneNode* GetParent() { return parent; }
	void SetParent(SceneNode* p) { this->parent = p; }

	void SetEnable(bool enable) { this->enable = enable; };
	bool IsEnable() { return enable; };

	inline Mesh* GetMesh() { return mesh; }
	void SetMesh(Mesh* m) { this->mesh = m; }

	void SetBoundingRadius(float radius) { this->boundingRadius = radius; }
	void SetDistanceFromCamera(float distance) { this->distanceFromCamera = distance; }
	void SetName(std::string name) {  this->name = name; };
	void SetColour(Vector4 colour) { this->colour = colour; }
	void SetModelScale(Vector3 scale) { modelScale = scale; }

	void SetLocalTransform(Matrix4 transform) { this->transform = transform; }

	void SetShader(std::string name) { shader = name; }
	std::string GetShader() { return shader; }

	void SetTexture(std::string name) { texture = name; }
	std::string GetTexture() { return texture; }

	std::string GetName() { return name; };
	Vector4 GetColour() { return colour; }
	Vector3 GetModelScale() { return modelScale; }

	Matrix4 GetWorldTransform() { return worldTransform; }
	Matrix4 GetLocalTransform() { return transform; }

	float GetBoundingRadius() { return boundingRadius; }
	float GetDistanceFromCamera() { return this->distanceFromCamera; }

	static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera < b->distanceFromCamera) ? true : false;
	}

protected:
	std::string name;
	bool enable;

	SceneNode* parent;
	Mesh* mesh;

	Matrix4 worldTransform;
	Matrix4 transform;

	Vector3 modelScale;
	Vector4 colour;

	std::map<std::string,SceneNode*> children;

	float boundingRadius;
	float distanceFromCamera;

	std::string texture;
	std::string shader;
};