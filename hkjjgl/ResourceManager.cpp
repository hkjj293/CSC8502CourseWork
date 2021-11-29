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

void ResourceManager::SetTexture(std::string name, GLuint texture) {
	textures[name] = texture;
}

void ResourceManager::SetFBO(std::string name, GLuint fbo) {
	FBOs[name] = fbo;
}

void ResourceManager::SetShader(std::string name, Shader* shader) {
	shaders[name] = shader;
}

void ResourceManager::SetFloat(std::string name, float num) {
	floats[name] = num;
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

GLuint ResourceManager::GetFBO(std::string name) {
	return FBOs[name];
}

Shader* ResourceManager::GetShader(std::string name) {
	return shaders[name];
}

float ResourceManager::GetFloat(std::string name) {
	return floats[name];
}