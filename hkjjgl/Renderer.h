#pragma once
#include "Window.h"
#include "Shader.h"
#include "Common.h"

#include "KHR\khrplatform.h"
#include "glad\glad.h"

#include "GL/GL.h"
#include "KHR/WGLext.h"

#include "SOIL/SOIL.h"

class Scene;

class Renderer {
public:
	Renderer(Window& parent);
	~Renderer();

	void Render(Scene scene);

protected:
};