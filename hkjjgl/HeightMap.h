#pragma once
#include <string>
#include "mesh.h"
#include "Vector3.h"

class HeightMap : public Mesh {
public:
	HeightMap(const std::string& name, bool negative = false);
	~HeightMap() {};

	Vector3 GetHeightmapSize() const { return heightmapSize; }
protected:
	Vector3 heightmapSize;
	GLuint glossMap;
};