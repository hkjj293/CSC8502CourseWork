#pragma once
#include "Vector3.h"

class Plane {
public:
	Plane() {};
	Plane(const Vector3& normal, float distance, bool mormalise = false);
	~Plane() {};

	bool SphereInPlane(const Vector3& position, float radius) const;

	Vector3 normal;
	float distance;
};