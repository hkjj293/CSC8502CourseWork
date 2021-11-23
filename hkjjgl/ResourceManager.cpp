#include "ResourceManager.h"

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {
	//TODO: delete OpenGL texture
	
	textures.clear();
	for (auto it = shaders.begin(); it != shaders.end(); ++it) {
		delete (*it).second;
	}
	shaders.clear();
}

void ResourceManager::AddTexture(std::string name, GLuint texture) {
	textures.emplace(name, texture);
}

void ResourceManager::AddShader(std::string name, Shader* shader) {
	shaders.emplace(name, shader);
}

void ResourceManager::RemoveTexture(std::string name) {
	textures.erase(name);
}

void ResourceManager::RemoveShader(std::string name) {
	shaders.erase(name);
}

GLuint ResourceManager::GetTexture(std::string name) {
	return textures[name];
}

Shader* ResourceManager::GetShader(std::string name) {
	return shaders[name];
}