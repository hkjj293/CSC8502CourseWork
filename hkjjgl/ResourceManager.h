#pragma once
#include <map>
#include "Renderer.h"

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	void AddTexture(std::string name, GLuint texture);
	void AddShader(std::string name, Shader* shader);

	void RemoveTexture(std::string name);
	void RemoveShader(std::string name);

	GLuint GetTexture(std::string name);
	Shader* GetShader(std::string name);


protected:
	std::map<std::string, GLuint> textures;
	std::map<std::string, Shader*> shaders;
};