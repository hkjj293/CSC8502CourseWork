#pragma once
#include <map>
#include "Renderer.h"

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	void SetTexture(std::string name, GLuint texture);
	void SetShader(std::string name, Shader* shader);
	void SetFloat(std::string name, float num);

	void RemoveTexture(std::string name);
	void RemoveShader(std::string name);

	GLuint GetTexture(std::string name);
	Shader* GetShader(std::string name);
	float GetFloat(std::string name);


protected:
	std::map<std::string, GLuint> textures;
	std::map<std::string, Shader*> shaders;
	std::map<std::string, float> floats;
};