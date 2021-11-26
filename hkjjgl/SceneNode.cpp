#include "SceneNode.h"

SceneNode::SceneNode(std::string name, Mesh* m, Vector4  colour) {
	this->name = name;
	this->mesh = m;
	this->colour = colour;
	transform = Matrix4();
	parent = nullptr;
	modelScale = Vector3(1,1,1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
	texture = "";
	shader = "";
	enable = true;
}

SceneNode::~SceneNode() {
	for (auto it = children.begin(); it != children.end(); ++it) {
		if ((*it).second) {
			delete (*it).second;
			(*it).second = nullptr;
		}
	}
	children.clear();
}

void SceneNode::AddChild(SceneNode* s) {
	children.emplace(s->GetName(),s);
	s->SetParent(this);
}

void SceneNode::RemoveChild(std::string name) {
	children.erase(name);
}

SceneNode* SceneNode::FindChild(std::string name) {
	return children[name];
}

SceneNode* SceneNode::GetChild(int index) {
	if (children.size() > index) {
		auto it = children.begin();
		for (int i = 0; i < index; i++) ++it;
		return (*it).second;
	}
	return nullptr;
}

std::map<std::string, SceneNode*>::iterator SceneNode::GetFirstChild() {
	return children.begin();
}

std::map<std::string, SceneNode*>::iterator SceneNode::GetChildrenEnd() {
	return children.end();
}

int SceneNode::GetChildSize() {
	return children.size();
}

void SceneNode::Update(float dt) {
	if (parent) {
		worldTransform = parent->worldTransform * transform;
	}
	else {
		worldTransform = transform;
	}
	for (auto i = children.begin(); i != children.end(); ++i) {
		(*i).second->Update(dt);
	}
}

void SceneNode::Draw() {
	if (mesh) {
		mesh->Draw();
	}
}