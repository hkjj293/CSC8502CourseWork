#include "Scene1.h"

#include "hkjjgl/SceneNode.h"
#include "hkjjgl/Renderer.h"
#include "hkjjgl/Camera.h"
#include "hkjjgl/ResourceManager.h"

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
	// Update Scene Nodes (position, rotation, materials, ...

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

	// Load Shaders to RM? Actually, I want to add Material instead... which include both shader and Textures and also rendering method
	Shader* bump = new Shader("bumpvertex.glsl", "bumpfragment.glsl");
	rManager->AddShader("bump", bump);

	// load FBOs to RM 
	
	// Construct Initial Scene Graph
	//Add Island
	SceneNode* island = new SceneNode("cheungChau", Mesh::LoadFromMeshFile("Meshes/CheungChau.obj"));
	root->AddChild(island);
	//Add Camera
	root->AddChild(mainCamera);
	//Add Water plane
	SceneNode* waterPlane = new SceneNode("waterPlane", Mesh::GenerateQuad());
	root->AddChild(waterPlane);
	//Add Light
	DirLight* directionalLight = new DirLight("directionalLight", Vector3(0, 0, 1), Vector4(1, 1, 1, 1));
	root->AddChild(directionalLight);
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