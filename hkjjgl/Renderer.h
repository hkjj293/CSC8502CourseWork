#pragma once
#include "Window.h"
#include "Shader.h"
#include "Common.h"

#include "KHR\khrplatform.h"
#include "glad\glad.h"

#include "GL/GL.h"
#include "KHR/WGLext.h"

#include "SOIL/SOIL.h"

#define OPENGL_DEBUGGING

class Scene;

class Renderer {
public:
	friend class Window;
	Renderer(Window& parent);
	~Renderer();

	bool HasInitialised() { return init; }

	virtual void Render(Scene* scene) {};
	void SwapBuffers();

protected:

	bool init;
	int width;
	int	height;
	virtual void Resize(int x, int y);

	void StartDebugGroup(const std::string& s) {
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, (GLsizei)s.length(), s.c_str());
	}

	void EndDebugGroup() {
		glPopDebugGroup();
	}
private:
	HDC	deviceContext;
	HGLRC renderContext;
#ifdef _DEBUG
	static void CALLBACK DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif
};