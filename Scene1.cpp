#include "Scene1.h"

#include "hkjjgl/SceneNode.h"
#include "hkjjgl/Renderer.h"
#include "hkjjgl/Camera.h"
#include "hkjjgl/ResourceManager.h"
#include "hkjjgl/Light.h"
#include "hkjjgl/HeightMap.h"

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
	timer->Tick();
	float diff = timer->GetTimeDeltaSeconds();
	std::cout << 1.0f / diff << " fps" << std::endl;
	mainCamera->Update(diff);
}

bool Scene1::Load() {
	// Init members
	root = new SceneNode();
	rManager = new ResourceManager();
	
	HeightMap* heightMap = new HeightMap("Textures/noise.png");
	Vector3  heightmapSize = heightMap->GetHeightmapSize();
	mainCamera = new  Camera("mainCamera",-45.0f, 0.0f, 0.0f, 45.0f, heightmapSize * Vector3(0.5f, 5.0f, 0.5f));

	// Load Textures to ResourceManager(RM)
	GLuint earthTex = SOIL_load_OGL_texture("Textures/Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	GLuint earthBump = SOIL_load_OGL_texture("Textures/Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	SetTextureRepeating(earthTex, true);
	SetTextureRepeating(earthBump, true);
	rManager->AddTexture("earthTex", earthTex);
	rManager->AddTexture("earthBump", earthBump);

	// Load Shaders to RM? Actually, I want to add Material instead... which include both shader and Textures and also rendering method
	Shader* bump = new Shader("bumpvertex.glsl", "bumpfragment.glsl");
	rManager->AddShader("bump", bump);

	// load FBOs to RM 
	
	// Construct Initial Scene Graph
	//Add Island
	SceneNode* island = new SceneNode("cheungChau", heightMap);
	root->AddChild(island);
	//Add Camera
	root->AddChild(mainCamera);
	//Add Water plane
	SceneNode* waterPlane = new SceneNode("waterPlane", Mesh::GenerateQuad());
	root->AddChild(waterPlane);
	//Add Light
	Light* directionalLight = new Light("directionalLight", heightmapSize * Vector3(0.95, 20.0f, -0.95), Vector4(1, 0.6, 0.3, 1), heightmapSize.x);
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

	return true;
}

void Scene1::SetTextureRepeating(GLuint target, bool repeating) {
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}