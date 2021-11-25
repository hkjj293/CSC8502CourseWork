#include "Renderer.h"
#include "Camera.h"
#include "Light.h"
#include "SceneNode.h"
#include "Frustum.h"
#include "ResourceManager.h"
#include "Scene.h"
#include <algorithm>

Renderer::Renderer(Window& w) {
	init = false;
	HWND windowHandle = w.GetHandle();

	if (!(deviceContext = GetDC(windowHandle))) {
		std::cout << "Renderer::Renderer(): Failed to create window!\n";
		return;
	}

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;	
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	GLuint		PixelFormat;
	if (!(PixelFormat = ChoosePixelFormat(deviceContext, &pfd))) {
		std::cout << "Renderer::Renderer(): Failed to choose a pixel format!\n";
		return;
	}

	if (!SetPixelFormat(deviceContext, PixelFormat, &pfd)) {
		std::cout << "Renderer::Renderer(): Failed to set a pixel format!\n";
		return;
	}

	HGLRC		tempContext;
	if (!(tempContext = wglCreateContext(deviceContext))) {	
		std::cout << "Renderer::Renderer(): Cannot create a temporary context!\n";
		wglDeleteContext(tempContext);
		return;
	}

	if (!wglMakeCurrent(deviceContext, tempContext)) {
		std::cout << "Renderer::Renderer(): Cannot set temporary context!\n";
		wglDeleteContext(tempContext);
		return;
	}

	if (!gladLoadGL()) {
		std::cout << "Renderer::Renderer(): Cannot initialise GLAD!\n";
		return;
	}

	char* ver = (char*)glGetString(GL_VERSION);
	int major = ver[0] - '0';
	int minor = ver[2] - '0';

	std::cout << "Renderer::Renderer(): Maximum OGL version supported is " << major << "." << minor << "\n";

	if (major < 3) {
		std::cout << "Renderer::Renderer(): Device does not support OpenGL 3.x!\n";
		wglDeleteContext(tempContext);
		return;
	}

	if (major == 3 && minor < 2) {
		std::cout << "Renderer::Renderer(): Device does not support OpenGL 3.2!\n";
		wglDeleteContext(tempContext);
		return;
	}

	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#ifdef OPENGL_DEBUGGING 
		| WGL_CONTEXT_DEBUG_BIT_ARB
#endif
		,WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	renderContext = wglCreateContextAttribsARB(deviceContext, 0, attribs);

	if (!renderContext || !wglMakeCurrent(deviceContext, renderContext)) {
		std::cout << "Renderer::Renderer(): Cannot set OpenGL 3 context!\n";
		wglDeleteContext(renderContext);
		wglDeleteContext(tempContext);
		return;
	}

	wglDeleteContext(tempContext);

#ifdef OPENGL_DEBUGGING
	glDebugMessageCallbackARB(&Renderer::DebugCallback, NULL);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
#endif

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	//currentShader = 0;

	w.SetRenderer(this);
	init = true;
}

Renderer::~Renderer() {
	wglDeleteContext(renderContext);
}

void Renderer::Render(Scene* scene) {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	
	// Repackage of render sequence
	// Get root & resource mangager
	SceneNode* root = scene->GetRoot();
	ResourceManager* rm = scene->GetResourceManager();

	//Rearrange scene nodes (group and sort)
	BuildNodeLists(scene);
	SortNodeLists();

	//Draw Nodes
	DrawNodes(scene);
	ClearNodeLists();

	//Post process?

	SwapBuffers();
}

void Renderer::SwapBuffers() {
#ifdef _WIN32
	::SwapBuffers(deviceContext);
#endif // _WIN32
}

void Renderer::BuildNodeLists(Scene* scene) {
	std::vector<std::map<std::string,SceneNode*>::iterator> posList;
	SceneNode* currNode = scene->GetRoot();
	posList.push_back(currNode->GetFirstChild());

	Camera* camera = dynamic_cast<Camera*>(scene->GetRoot()->FindChild("mainCamera"));

	Frustum frameFrustum;
	frameFrustum.FromMatrix(camera->BuildProjMatrix((float)width,(float)height) * camera->BuildViewMatrix());

	while (posList.size() > 0) {
		//std::cout << currNode->GetName() << std::endl;
		if (posList[posList.size() - 1] == currNode->GetFirstChild() && frameFrustum.InsideFrustum(*currNode)) {
			Vector3 dir = currNode->GetWorldTransform().GetPositionVector() - camera->position;
			if (currNode->GetName() == "skybox") {
				currNode->SetDistanceFromCamera(-1);
			}
			else {
				currNode->SetDistanceFromCamera(Vector3::Dot(dir, dir));
			}
			if (currNode->GetColour().w < 1.0f) {
				transparentNodeList.push_back(currNode);
			}
			else {
				nodeList.push_back(currNode);
			}
		}
		if (posList[posList.size() - 1] != currNode->GetChildrenEnd()) {
			currNode = (*posList[posList.size() - 1]).second;
			posList[posList.size() - 1]++;
			posList.push_back(currNode->GetFirstChild());
		}
		else {
			posList.pop_back();
			currNode = currNode->GetParent();
		}
	}
}

void Renderer::SortNodeLists() {
	std::sort(transparentNodeList.rbegin(), transparentNodeList.rend(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes(Scene* scene) {
	for (const auto& i : nodeList) {
		DrawNode(i, scene);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (const auto& i : transparentNodeList) {
		DrawNode(i, scene);
	}
	glDisable(GL_BLEND);
}

void Renderer::DrawNode(SceneNode* n,Scene* scene) {
	//std::cout<< n->GetName() <<std::endl;
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	if (n->GetMesh()) {
		GLuint currShader = scene->GetResourceManager()->GetShader(n->GetShader())->GetProgram();
		glUseProgram(currShader);
		Camera* camera = dynamic_cast<Camera*>(scene->GetRoot()->FindChild("mainCamera"));
		Light* light = dynamic_cast<Light*>(scene->GetRoot()->FindChild("lightGimbal")->FindChild("directionalLight"));

		Matrix4 viewMatrix = Matrix4();
		Matrix4 modelMatrix = Matrix4();
		Matrix4 projMatrix = Matrix4();

		modelMatrix = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		projMatrix = camera->BuildProjMatrix((float)width, (float)height);
		viewMatrix = camera->BuildViewMatrix();

		glUniformMatrix4fv(glGetUniformLocation(currShader, "viewMatrix"), 1, false, viewMatrix.values);
		glUniformMatrix4fv(glGetUniformLocation(currShader, "modelMatrix"), 1, false, modelMatrix.values);
		glUniformMatrix4fv(glGetUniformLocation(currShader, "projMatrix"), 1, false, projMatrix.values);

		if (camera) {
			glUniform3fv(glGetUniformLocation(currShader, "cameraPos"), 1, (float*)&(camera->position));
		}		
		glUniform1i(glGetUniformLocation(currShader, "isPoint"), 0);
		if (light) {
			Vector3 p = light->GetPosition();
			Vector4 c = light->GetColour();
			glUniform3fv(glGetUniformLocation(currShader, "lightPos"), 1, (float*)&p);
			glUniform4fv(glGetUniformLocation(currShader, "lightColour"), 1, (float*)&c);
			glUniform1f(glGetUniformLocation(currShader, "lightRadius"), light->GetRadius());
		}
		if (n->GetTexture() == "cubeMap") {
			glUniform1i(glGetUniformLocation(currShader, "cubeTex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, scene->GetResourceManager()->GetTexture(n->GetTexture()));
			glUniform1f(glGetUniformLocation(currShader, "rot"), scene->GetResourceManager()->GetFloat("rotation"));
			Matrix4 rotation = Matrix4::Rotation(scene->GetResourceManager()->GetFloat("rotation"), Vector3(0, 0 ,1));
			glUniformMatrix4fv(glGetUniformLocation(currShader, "rotation"), 1, false, rotation.values);
			glDisable(GL_DEPTH_TEST);
		}
		else if(n->GetName() == "cheungChau") {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glUniform1f(glGetUniformLocation(currShader, "grassLine"), 46);
			glUniform1i(glGetUniformLocation(currShader, "diffuseTex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16);
			glBindTexture(GL_TEXTURE_2D, scene->GetResourceManager()->GetTexture(n->GetTexture()));

			glUniform1i(glGetUniformLocation(currShader, "diffuseTex2"), 2);
			glActiveTexture(GL_TEXTURE2);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16);
			glBindTexture(GL_TEXTURE_2D, scene->GetResourceManager()->GetTexture("grassTex"));

			glUniform1i(glGetUniformLocation(currShader, "bumpTex"), 1);
			glActiveTexture(GL_TEXTURE1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16);
			glBindTexture(GL_TEXTURE_2D, scene->GetResourceManager()->GetTexture("earthBump"));


			glUniform1i(glGetUniformLocation(currShader, "bumpTex2"), 3);
			glActiveTexture(GL_TEXTURE3);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16);
			glBindTexture(GL_TEXTURE_2D, scene->GetResourceManager()->GetTexture("grassBump"));
		}
		else {
			glUniform1f(glGetUniformLocation(currShader, "rot"), scene->GetResourceManager()->GetFloat("rotation"));

			Matrix4 rotation = Matrix4::Rotation(scene->GetResourceManager()->GetFloat("rotation"), Vector3(0, 0, 1));
			glUniformMatrix4fv(glGetUniformLocation(currShader, "textureMatrix"), 1, false, rotation.values);

			glUniform1f(glGetUniformLocation(currShader, "ratio"), 0.3);

			glUniform1i(glGetUniformLocation(currShader, "cubeTex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, scene->GetResourceManager()->GetTexture("cubeMap"));

			glUniform1i(glGetUniformLocation(currShader, "diffuseTex"), 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, scene->GetResourceManager()->GetTexture(n->GetTexture()));

		}
		n->Draw();
	}
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer::Resize(int x, int y) {
	width = std::max(x, 1);
	height = std::max(y, 1);
	glViewport(0, 0, width, height);
}

#ifdef OPENGL_DEBUGGING
void Renderer::DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::string sourceName;
	std::string typeName;
	std::string severityName;

	if (type == GL_DEBUG_TYPE_OTHER_ARB) {
		return;
	}

	switch (source) {
	case GL_DEBUG_SOURCE_API_ARB: sourceName = "Source(OpenGL)"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: sourceName = "Source(Window System)"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: sourceName = "Source(Shader Compiler)"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: sourceName = "Source(Third Party)"; break;
	case GL_DEBUG_SOURCE_APPLICATION_ARB: sourceName = "Source(Application)"; break;
	case GL_DEBUG_SOURCE_OTHER_ARB: sourceName = "Source(Other)"; break;
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR_ARB: typeName = "Type(Error)"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: typeName = "Type(Deprecated Behaviour)"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: typeName = "Type(Undefined Behaviour)"; break;
	case GL_DEBUG_TYPE_PORTABILITY_ARB: typeName = "Type(Portability)"; break;
	case GL_DEBUG_TYPE_PERFORMANCE_ARB: typeName = "Type(Performance)"; break;
	case GL_DEBUG_TYPE_OTHER_ARB: typeName = "Type(Other)"; break;
	default: {
		return;
	}
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH_ARB: severityName = "Priority(High)"; break;
	case GL_DEBUG_SEVERITY_MEDIUM_ARB: severityName = "Priority(Medium)"; break;
	case GL_DEBUG_SEVERITY_LOW_ARB: severityName = "Priority(Low)"; break;
	}

	std::cout << "OpenGL Debug Output: " + sourceName + ", " + typeName + ", " + severityName + ", " + std::string(message) << "\n";
}
#endif