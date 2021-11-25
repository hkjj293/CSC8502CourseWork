#pragma once
#include "Window.h"
#include "Shader.h"
#include "Common.h"
#include "Vector3.h"
#include "Vector4.h"

#include <string>
#include <fstream>
#include <vector>

#include "KHR\khrplatform.h"
#include "glad\glad.h"

#include "GL/GL.h"
#include "KHR/WGLext.h"

#include "SOIL/SOIL.h"

#ifdef _DEBUG
#define OPENGL_DEBUGGING
#endif // DEBUG



class Scene;
class SceneNode;
class Light;
class GameTimer;

class Renderer {
public:
	friend class Window;
	Renderer(Window& parent);
	~Renderer();

	bool HasInitialised() { return init; }

	virtual void Render(Scene* scene);
	void SwapBuffers();

protected:

	bool init;
	int width;
	int	height;
	GameTimer* t;

	/*********** Will be modified in the future to adapt layers and shader based rendering***********/
	std::vector <SceneNode*> transparentNodeList;
	std::vector <SceneNode*> nodeList;

	std::vector <Light*> lightList;
	std::vector <Vector3> lightPos;
	std::vector <Vector3> lightDirection;
	std::vector <Vector4> lightColour;
	std::vector <float> lightRadius;
	std::vector <int> lightType;

	void BuildNodeLists(Scene* scene);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes(Scene* scene);
	void DrawNode(SceneNode* n, Scene* scene);
	/**********************/
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