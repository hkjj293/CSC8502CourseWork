#include "Scene1.h"

#include "hkjjgl/SceneNode.h"
#include "hkjjgl/Renderer.h"
#include "hkjjgl/Camera.h"
#include "hkjjgl/ResourceManager.h"
#include "hkjjgl/Light.h"
#include "hkjjgl/HeightMap.h"

Scene1::Scene1(): Scene("scene1") {
	mainCamera = nullptr;
}

Scene1::~Scene1() {
	Unload();
}

void Scene1::Update() {
	// Update Scene Nodes (position, rotation, materials, ...
	timer->Tick();
	float diff = timer->GetTimeDeltaSeconds();
	//std::cout << 1.0f / diff << " fps" << std::endl;

	if (!root->FindChild("mainCamera")) mainCamera == nullptr;
	if(mainCamera) mainCamera->Update(diff);
	rManager->SetFloat("rotation", rManager->GetFloat("rotation") + (20 * diff));
	if (rManager->GetFloat("rotation") > 360.0f) {
		rManager->SetFloat("rotation", rManager->GetFloat("rotation") - 360);
	}
	root->FindChild("lightGimbal")->SetLocalTransform(Matrix4::Rotation(-rManager->GetFloat("rotation") , Vector3(0, 0, 1)));
	dynamic_cast<Light*>(root->FindChild("lightGimbal")->FindChild("directionalLight"))->SetDirection(Matrix4::Rotation(-rManager->GetFloat("rotation"), Vector3(0, 0, 1))* Vector3(1, 0, 0));

	Light* camLight = dynamic_cast<Light*>(root->FindChild("mainCamera")->FindChild("camLight"));

	camLight->SetDirection((mainCamera->BuildTransformMatrix() * Vector4(0,0,-1,1)).ToVector3());
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_L)) {
		camLight->SetEnable(!camLight->IsEnable());
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C)) {
		camLight->SetType((camLight->GetType() + 1)%4);
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
		camLight->SetColour(Vector4(camLight->GetColour().x, camLight->GetColour().y, camLight->GetColour().z, camLight->GetColour().w + (1000 * diff)));
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN)) {
		camLight->SetColour(Vector4(camLight->GetColour().x, camLight->GetColour().y, camLight->GetColour().z, camLight->GetColour().w - (1000 * diff)));
	}

	if (mainCamera) {
		float  speed = 100.0f * diff;
		mainCamera->pitch -= (Window::GetMouse()->GetRelativePosition().y) * 3.0f;
		mainCamera->yaw -= (Window::GetMouse()->GetRelativePosition().x) * 3.0f;
		mainCamera->fov -= (Window::GetMouse()->GetWheelMovement()) * 3.0f;
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_Q)) {
			mainCamera->roll -= speed;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_E)) {
			mainCamera->roll += speed;
		}

		mainCamera->pitch = std::min(mainCamera->pitch, 90.0f);
		mainCamera->pitch = std::max(mainCamera->pitch, -90.0f);

		if (mainCamera->yaw < 0) {
			mainCamera->yaw += 360.0f;
		}
		if (mainCamera->yaw > 360.0f) {
			mainCamera->yaw -= 360.0f;
		}

		if (mainCamera->roll < 0) {
			mainCamera->roll += 360.0f;
		}
		if (mainCamera->roll > 360.0f) {
			mainCamera->roll -= 360.0f;
		}

		Matrix4  rotation = Matrix4::Rotation(mainCamera->yaw, Vector3(0, 1, 0)) *
			Matrix4::Rotation(mainCamera->pitch, Vector3(1, 0, 0)) *
			Matrix4::Rotation(mainCamera->roll, Vector3(0, 0, -1));
		Vector3  forward = rotation * Vector3(0, 0, -1);
		Vector3  right = rotation * Vector3(1, 0, 0);

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
			mainCamera->position += forward * speed;
			mainCamera->position.y += sin(2.0f * PI * timer->GetTotalTimeMSec() / 2) * 0.051;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
			mainCamera->position -= forward * speed;
			mainCamera->position.y += sin(2.0f * PI * timer->GetTotalTimeMSec() / 2) * 0.051;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
			mainCamera->position -= right * speed;
			mainCamera->position.y += sin(2.0f * PI * timer->GetTotalTimeMSec() / 2) * 0.051;
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
			mainCamera->position += right * speed;
			mainCamera->position.y += sin(2.0f * PI * timer->GetTotalTimeMSec() / 2) * 0.051;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
			mainCamera->position.y += speed;
		}
	}

	root->Update(diff);
}

bool Scene1::Load() {
	// Init members
	root = new SceneNode("root");

	rManager = new ResourceManager();
	
	HeightMap* heightMap = new HeightMap(TEXTUREDIR"noise.png");
	//heightMap->SetType(GL_PATCHES);
	Vector3  heightmapSize = heightMap->GetHeightmapSize();
	std::cout << heightmapSize;
	//mainCamera = new Camera("mainCamera",-45.0f, 0.0f, 0.0f, 45.0f, heightmapSize * Vector3(0.5f, 5.0f, 0.5f));

	// Load Textures to ResourceManager(RM)
	GLuint waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water1.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	GLuint earthTex = SOIL_load_OGL_texture(TEXTUREDIR"Sand_2_Diffuse.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	GLuint earthBump = SOIL_load_OGL_texture(TEXTUREDIR"Sand_2_Normal.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	GLuint grassTex = SOIL_load_OGL_texture(TEXTUREDIR"grass_Diffuse.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	GLuint grassBump = SOIL_load_OGL_texture(TEXTUREDIR"grass_Normal.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	GLuint cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg", TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	GLuint cubeMap2 = SOIL_load_OGL_cubemap(TEXTUREDIR"starry_west.png", TEXTUREDIR"starry_east.png", TEXTUREDIR"starry_up.png", TEXTUREDIR"starry_down.png", TEXTUREDIR"starry_north.png", TEXTUREDIR"starry_south.png", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	
	if (!earthTex || !earthBump || !cubeMap || !cubeMap2 || !waterTex || !grassTex || !grassBump) {
		std::cout << glGetError() << std::endl;
		return false;
	}

	SetTextureRepeating(waterTex, true);
	SetTextureRepeating(earthTex, true);
	SetTextureRepeating(earthBump, true);
	SetTextureRepeating(grassTex, true);
	SetTextureRepeating(grassBump, true);
	
	rManager->SetTexture("waterTex", waterTex);
	rManager->SetTexture("earthTex", earthTex);
	rManager->SetTexture("earthBump", earthBump);
	rManager->SetTexture("grassTex", grassTex);
	rManager->SetTexture("grassBump", grassBump);
	rManager->SetTexture("cubeMap", cubeMap);
	rManager->SetTexture("cubeMap2", cubeMap2);

	// Load Shaders to RM? Actually, I want to add Material instead... which include both shader and Textures and also rendering method
	Shader* bump = new Shader("bumpvertex.glsl", "bumpfragment.glsl");
	Shader* skybox = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
	Shader* reflect = new Shader("reflectVertex.glsl", "reflectFragment.glsl");
	Shader* tess = new Shader("tessVert.glsl", "displaceFrag.glsl", "", "displaceTCS.glsl", "displaceTES.glsl");
	Shader* processShader = new  Shader("textureShader.glsl", "processfrag.glsl");
	if (!skybox->LoadSuccess() || !bump->LoadSuccess() || !reflect->LoadSuccess() || !tess->LoadSuccess() || !processShader->LoadSuccess()) {
		return false;
	}
	rManager->SetShader("bump", bump);
	rManager->SetShader("skybox", skybox);
	rManager->SetShader("reflect", reflect);
	rManager->SetShader("tess", tess);
	rManager->SetShader("proc", processShader);

	// Set Global variables across objects that uses the same resource manager
	rManager->SetFloat("rotation", 90);

	// load FBOs to RM 
	GLuint fBO1;
	glGenTextures(1, &fBO1);
	glBindTexture(GL_TEXTURE_2D, fBO1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	rManager->SetTexture("fBO1", fBO1);

	// Construct Initial Scene Graph
	// Add cube map
	SceneNode* skyboxQuad = new SceneNode("skybox", Mesh::GenerateQuad());
	skyboxQuad->SetLocalTransform(Matrix4::Translation(Vector3(0,0,-1)) * Matrix4::Rotation(90,Vector3(1,0,0)));
	skyboxQuad->SetBoundingRadius(INFINITY);
	skyboxQuad->SetTexture("cubeMap2");
	skyboxQuad->SetShader("skybox");
	root->AddChild(skyboxQuad);
	// Add Mini map
	SceneNode* miniMap = new SceneNode("miniMap", Mesh::GenerateQuad());
	float scale = 0.125;
	miniMap->SetModelScale(Vector3(scale, scale, scale * (float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN)));
	float posX = (2.0f - (scale * 2.0f)) / 2.0f;
	float posY = (2.0f - (scale * (float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN) * 2.0f)) / 2.0f;
	miniMap->SetLocalTransform(Matrix4::Translation(Vector3(posX, posY , -1)) * Matrix4::Rotation(90, Vector3(1, 0, 0)));
	miniMap->SetBoundingRadius(INFINITY);
	miniMap->SetTexture("cubeMap2");
	miniMap->SetShader("skybox");
	root->AddChild(miniMap);

	//Add Island
	SceneNode* cheungChau = new SceneNode("cheungChau", heightMap);
	cheungChau->SetTexture("earthTex");
	cheungChau->SetShader("bump");
	cheungChau->SetBoundingRadius(sqrt(pow(heightMap->GetHeightmapSize().x,2)*2)/2);
	root->AddChild(cheungChau);

	//Add Camera
	root->AddChild(new Camera("mainCamera", -45.0f, 0.0f, 0.0f, 45.0f, Vector3(0.0f, 128.0f, 0.0f)));
	mainCamera = dynamic_cast<Camera*>(root->FindChild("mainCamera"));
	mainCamera->AddChild(new Light("camLight", 2, Vector3(0, 0, 0.0f), Vector3(0, 0, 0), Vector4(1, 1, 1, 1000), 100));

	//Add Water plane
	Mesh* water = Mesh::GenerateQuad();
	//water->SetType(GL_PATCHES);
	SceneNode* waterPlane = new SceneNode("waterPlane", water, Vector4(1, 1, 1, 0.3f));
	waterPlane->SetModelScale(heightMap->GetHeightmapSize());
	waterPlane->SetTexture("waterTex");
	waterPlane->SetShader("reflect");
	waterPlane->SetBoundingRadius(sqrt(pow(heightMap->GetHeightmapSize().x , 2) * 2)/2);
	root->AddChild(waterPlane);

	//Add Light
	root->AddChild(new SceneNode("lightGimbal"));
	root->FindChild("lightGimbal")->AddChild(new Light("directionalLight", DIRECTIONAL_LIGHT, Vector3(0, 0, 0), Vector3(0, -1, 0), Vector4(1, 0.6, 0.3,1000), heightmapSize.x));
	root->FindChild("lightGimbal")->FindChild("directionalLight")->SetLocalTransform(Matrix4::Translation(Vector3(0.f, 1000.0f, 0.0f)));
	
	for (int i = 0; i < 1; i++)
	{
		std::string name = "pointLight" + std::to_string(i);
		root->AddChild(new Light(name, 0, Vector3(0, 0, 0.0f), Vector3(0, 1, 0), Vector4(1, 0.01 * (rand() % 100), 0.3, 1000), 100));
		root->FindChild(name)->SetLocalTransform(Matrix4::Translation(Vector3(((rand() % 100)+20) * i, 100.0f, ((rand() % 100) + 20) * i)));
	}
	
	return true;
}

bool Scene1::Unload() {
	if (this->root)
		delete this->root;
	root = nullptr;

	if (this->rManager)
		delete this->rManager;
	rManager = nullptr;

	return true;
}

void Scene1::SetTextureRepeating(GLuint target, bool repeating) {
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}