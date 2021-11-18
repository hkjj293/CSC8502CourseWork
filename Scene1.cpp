#include "Scene1.h"

#include "hkjjgl/Common.h"
#include "hkjjgl/SceneNode.h"
#include "hkjjgl/Shader.h"

#include "KHR\khrplatform.h"
#include "glad\glad.h"

#include "GL/GL.h"
#include "KHR/WGLext.h"

#include "SOIL/SOIL.h"

Scene1::Scene1(): Scene("scene1") {
	init = false;
	forceQuit = false;
	nextScene = name;
	root = nullptr;
	rManager = nullptr;
	mainCamera = nullptr;
	init = true;
}

Scene1::~Scene1() {
	Scene::~Scene();
}

void Scene1::Update() {
	
}

bool Scene1::Load() {
	// Init members
	root = new SceneNode();
	rManager = new ResourceManager();
	mainCamera = new Camera();

	// Load Textures to ResourceManager(RM)
	GLuint earthTex = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	GLuint earthBump = SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	rManager->AddTexture("Barren Reds.JPG", earthTex);
	rManager->AddTexture("Barren RedsDOT3.JPG", earthBump);

	// Load Shaders to RM
	Shader* bump = new  Shader("bumpvertex.glsl", "bumpfragment.glsl");
	rManager->AddShader("bump", bump);

	// load FBOs
	
	// Construct Initial Scene Graph
	root->AddChild(new SceneNode(new Mesh("Meshes/CheungChau.obj")));
	root->AddChild(new SceneNode(mainCamera));
	return true;
}

bool Scene1::Unload() {
	if (this->root)
		delete this->root;
	root = nullptr;

	if (this->rManager)
		delete this->rManager;
	rManager = nullptr;

	if (this->mainCamera)
		delete this->mainCamera;
	mainCamera = nullptr;
}