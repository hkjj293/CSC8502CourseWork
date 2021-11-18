#include "SceneNode.h"

SceneNode::SceneNode(std::string name, Mesh* m, Vector4  colour) {
	this->mesh = m;
	this->colour = colour;
	parent = NULL;
	modelScale = Vector3(1,1,1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
	texture = 0;
}

SceneNode::~SceneNode() {
	for (int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
	//children.clear();
}

void SceneNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->parent = this;
}

void SceneNode::RemoveChild(std::string name) {
	for (auto it = children.begin(); it != children.end(); ++it) {
		if ((*it)->GetName() == name) {
			delete (*it);
			children.erase(it);
			return;
		}
	}
}


void SceneNode::Update(float dt) {
	if (parent) {
		worldTransform = parent->worldTransform * transform;
	}
	else {
		worldTransform = transform;
	}
	for (auto i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(dt);
	}
}