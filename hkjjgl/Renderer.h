#pragma once
#include "Window.h"

class Scene;

class Renderer {
public:
	Renderer(Window& parent);
	~Renderer();

	void Render(Scene scene);

protected:
};