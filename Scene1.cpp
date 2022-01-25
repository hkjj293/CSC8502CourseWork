#include "Scene1.h"

#include "hkjjgl/SceneNode.h"
#include "hkjjgl/Renderer.h"
#include "hkjjgl/Camera.h"
#include "hkjjgl/ResourceManager.h"
#include "hkjjgl/Light.h"
#include "hkjjgl/HeightMap.h"

Scene1::Scene1(): Scene("scene1") {
	mainCamera = nullptr;
	lightCount = 0;
}

Scene1::~Scene1() {
	Unload();
}

void Scene1::Update() {
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		forceQuit = true;
	}
	// Update Scene Nodes (position, rotation, materials, ...
	timer->Tick();
	float diff = timer->GetTimeDeltaSeconds();
	//std::cout << 1.0f / diff << " fps" << std::endl;
	if (!root->FindChild("mainCamera")) mainCamera == nullptr;
	if(mainCamera) mainCamera->Update(diff);
	rManager->SetFloat("rotation", rManager->GetFloat("rotation") + ( diff));
	if (rManager->GetFloat("rotation") > 360.0f) {
		rManager->SetFloat("rotation", 0);
	}
	root->FindChild("lightGimbal")->SetLocalTransform(Matrix4::Rotation(-rManager->GetFloat("rotation") , Vector3(0, 0, 1)));
	dynamic_cast<Light*>(root->FindChild("lightGimbal")->FindChild("directionalLight"))->SetDirection(Matrix4::Rotation(-rManager->GetFloat("rotation"), Vector3(0, 0, 1))* Vector3(1, 0, 0));

	if (mainCamera) {
		Light* camLight = dynamic_cast<Light*>(root->FindChild("mainCamera")->FindChild("camLight"));

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_L)) {
			camLight->SetEnable(!camLight->IsEnable());
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C)) {
			camLight->SetType((camLight->GetType() + 1) % 4);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
			camLight->SetColour(Vector4(camLight->GetColour().x, camLight->GetColour().y, camLight->GetColour().z, camLight->GetColour().w + (1000 * diff)));
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN)) {
			camLight->SetColour(Vector4(camLight->GetColour().x, camLight->GetColour().y, camLight->GetColour().z, camLight->GetColour().w - (1000 * diff)));
		}

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

		Vector3  forward = mainCamera->BuildRotationMatrix() * Vector3(0, 0, -1);
		Vector3  right = mainCamera->BuildRotationMatrix() * Vector3(1, 0, 0);

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
		mainCamera->SetLocalTransform(mainCamera->BuildTransformMatrix());
		root->FindChild("miniMapCamera")->SetLocalTransform(Matrix4::Translation(Vector3(mainCamera->position.x, 1000, mainCamera->position.z)));
		dynamic_cast<Camera*>(root->FindChild("miniMapCamera"))->yaw = mainCamera->yaw;
		//root->FindChild("waterPlane")->SetLocalTransform(Matrix4::Translation(Vector3(mainCamera->position.x,0, mainCamera->position.z)));

		camLight->SetDirection((mainCamera->BuildRotationMatrix() * Vector4(0, 0, -1, 1)).ToVector3());
	}
	if (Window::GetMouse()->ButtonTriggered(MOUSE_LEFT)) {
		lightCount++;
		root->AddChild(new Light("Mouse"+std::to_string(lightCount), 3, Vector3(0, 0, 0.0f), Vector3(0, 1, 0), Vector4(1, 0.01 * (rand() % 100), 0.3, 1000), 10));
		root->FindChild("Mouse" + std::to_string(lightCount))->SetLocalTransform(mainCamera->BuildTransformMatrix());
		dynamic_cast<Light*>(root->FindChild("Mouse" + std::to_string(lightCount)))->SetDirection((mainCamera->BuildRotationMatrix() * Vector4(0, 0, -1, 1)).ToVector3());
	}
	if (Window::GetMouse()->ButtonTriggered(MOUSE_RIGHT)) {
		if (lightCount > 0) {
			root->RemoveChild("Mouse" + std::to_string(lightCount));
			lightCount--;
		}
	}
	root->Update(diff);
}

bool Scene1::Load() {
	// Init common var
	int width = std::min(GetSystemMetrics(SM_CXSCREEN), 1920);
	int height = std::min(GetSystemMetrics(SM_CYSCREEN), 1080);
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
	//SetTextureFilter(waterTex);
	SetTextureRepeating(earthTex, true);
	SetTextureFilter(earthTex);
	SetTextureRepeating(earthBump, true);
	SetTextureFilter(earthBump);
	SetTextureRepeating(grassTex, true);
	SetTextureFilter(grassTex);
	SetTextureRepeating(grassBump, true);
	SetTextureFilter(grassBump);

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
	Shader* tess = new Shader("tessVert.glsl", "reflectFragment.glsl", "", "displaceTCS.glsl", "displaceTES.glsl");
	Shader* processShader = new  Shader("textureShader.glsl", "processfrag.glsl");
	Shader* tex = new Shader("textureShader.glsl", "texturedFragment.glsl");
	if (!skybox->LoadSuccess() || !bump->LoadSuccess() || !reflect->LoadSuccess() || !tess->LoadSuccess() || !processShader->LoadSuccess()) {
		return false;
	}
	rManager->SetShader("bump", bump);
	rManager->SetShader("skybox", skybox);
	rManager->SetShader("reflect", reflect);
	rManager->SetShader("tess", tess);
	rManager->SetShader("proc", processShader);
	rManager->SetShader("tex", tex);

	// Set Global variables across objects that uses the same resource manager
	rManager->SetFloat("rotation", 90);
	rManager->SetFloat("scale", 1);

	// load FBO Textures to RM 
	GLuint FBOTex1;
	glGenTextures(1, &FBOTex1);
	glBindTexture(GL_TEXTURE_2D, FBOTex1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,width,height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	rManager->SetTexture("FBOTex1", FBOTex1);

	GLuint bufferDepthTex;
	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	rManager->SetTexture("bufferDepthTex", bufferDepthTex);

	// load FBOs
	GLuint bufferFBO;
	glGenFramebuffers(1, &bufferFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rManager->GetTexture("bufferDepthTex"), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, rManager->GetTexture("bufferDepthTex"), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rManager->GetTexture("FBOTex1"), 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !rManager->GetTexture("FBOTex1") || !rManager->GetTexture("bufferDepthTex")) {
		std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << glGetError() << std::endl;
		return false;
	}
	rManager->SetFBO("bufferFBO", bufferFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
	float scale = 0.25;
	miniMap->SetModelScale(Vector3(scale, 1, scale * (float)width / (float)height));
	float posX = (2.0f - (scale * 2.0f)) / 2.0f;
	float posY = (2.0f - ((scale * (float)width / (float)height) * 2.0f)) / 2.0f;
	miniMap->SetLocalTransform(Matrix4::Translation(Vector3(posX, posY , -1)) * Matrix4::Rotation(90, Vector3(1, 0, 0)));
	miniMap->SetBoundingRadius(INFINITY);
	miniMap->SetTexture("FBOTex1");
	miniMap->SetShader("tex");
	root->AddChild(miniMap);

	//Add Island
	SceneNode* cheungChau = new SceneNode("cheungChau", heightMap);
	cheungChau->SetTexture("earthTex");
	cheungChau->SetShader("bump");
	cheungChau->SetBoundingRadius(sqrt(pow(heightMap->GetHeightmapSize().x,2)*2)/2);
	root->AddChild(cheungChau);

	//Add Camera
	root->AddChild(new Camera("mainCamera", -45.0f, 0.0f, 0.0f, 45.0f, Vector3(0.0f, 128.0f, 0.0f)));
	root->FindChild("mainCamera")->SetLocalTransform(Matrix4::Translation(Vector3(0, 128, 0)));

	mainCamera = dynamic_cast<Camera*>(root->FindChild("mainCamera"));
	mainCamera->AddChild(new Light("camLight", 2, Vector3(0, 0, 0.0f), Vector3(0, 0, 0), Vector4(1, 1, 1, 1000), 100));
	
	root->AddChild(new Camera("miniMapCamera",-90.0f, 0.0f, 0.0f, 45.0f, Vector3(0.0f, 1000.0f, 0.0f)));
	root->FindChild("miniMapCamera")->SetLocalTransform(Matrix4::Translation(Vector3(0,1000, 0)));

	//Add Water plane
	Mesh* water = Mesh::GenerateQuad();
	water->SetType(GL_PATCHES);
	//water->SetType(GL_PATCHES);
	SceneNode* waterPlane = new SceneNode("waterPlane", water, Vector4(1, 1, 1, 0.3f));
	waterPlane->SetModelScale(heightMap->GetHeightmapSize() * 0.25f);
	//waterPlane->SetLocalTransform(Matrix4::Translation(heightMap->GetHeightmapSize()));
	waterPlane->SetTexture("waterTex");
	waterPlane->SetShader("tess");// reflect");
	waterPlane->SetBoundingRadius(INFINITY);// sqrt(pow(heightMap->GetHeightmapSize().x, 2) * 2) / 2);
	root->AddChild(waterPlane);

	//Add Light
	root->AddChild(new SceneNode("lightGimbal"));
	root->FindChild("lightGimbal")->AddChild(new Light("directionalLight", DIRECTIONAL_LIGHT, Vector3(0, 0, 0), Vector3(0, -1, 0), Vector4(1, 0.6, 0.3,1000), heightmapSize.x));
	root->FindChild("lightGimbal")->FindChild("directionalLight")->SetLocalTransform(Matrix4::Translation(Vector3(0.f, 2000.0f, 0.0f)));
	
	for (int i = 0; i < 1; i++)
	{
		std::string name = "pointLight" + std::to_string(i);
		root->AddChild(new Light(name, 3, Vector3(0, 0, 0.0f), Vector3(0, 1, 0), Vector4(1, 0.01 * (rand() % 100), 0.3, 1000), 100));
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

void Scene1::SetTextureFilter(GLuint target) {
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}