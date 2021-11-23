#include "Renderer.h"
#include "Scene.h"

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

	SwapBuffers();
}

void Renderer::SwapBuffers() {
#ifdef _WIN32
	::SwapBuffers(deviceContext);
#endif // _WIN32
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