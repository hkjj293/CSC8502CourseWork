#pragma once
#include "Plane.h"

class SceneNode;
class Matrix4;

class Frustum {
public:
	Frustum() {};
	~Frustum() {};

	void FromMatrix(const Matrix4& mvp);
	bool InsideFrustum(SceneNode& n);

protected:
	Plane planes[6];
};