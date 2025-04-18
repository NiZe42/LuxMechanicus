#include "Game.h"
#include <iostream>
#include <filesystem>

Renderer* Game::mRenderer = nullptr;
float Game::mLastXMousePos = 0;
float Game::mLastYMousePos = 0;

Game::Game() {
	Initialize();
}

Game::~Game() {
	delete mRenderer;

	if (mScenes.size() == 0)
		return;

	for (auto iterator = mScenes.begin(); iterator != mScenes.end(); iterator++) {
		delete(*iterator);
	}
	
	MeshCache::ClearCache();
	mScenes.clear();
}

void Game::Initialize() {
	std::cout << "Initializing engine..." << std::endl;
	std::cout << "Current root path is:  " << std::filesystem::current_path() << std::endl;

	Environment::SetRootPath(std::filesystem::current_path().string().c_str());

	mScenes = {};

	InitializeGLFW();
	InitializeWindow();
	InitializeGLAD();	

	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(mWindow, MouseCallback);

	mLastFrameTime = static_cast<float>(glfwGetTime());
	mFrameCounter = 0;

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	Scene* mainScene = new Scene();
	AddScene(mainScene);

	for (int i = 0; i < 10; i++)
	{
		GameObject* square = new GameObject(
			cubePositions[i],
			glm::vec3(rand() % 360, rand() % 360, rand() % 360),
			glm::vec3(0.7f),
			(std::string(Environment::GetRootPath()) + "/Shaders/ColorVert.glsl").c_str(),
			(std::string(Environment::GetRootPath()) + "/Shaders/ColorFrag.glsl").c_str(),
			(std::string(Environment::GetRootPath()) + "/Textures/bricks.jpg").c_str());

		Mesh* mesh;
		int randomIndex = rand() % 2;
		if (randomIndex == 0)
			mesh = MeshCache::GetMesh((std::string(Environment::GetRootPath()) + "/Models/teapot_flat.obj").c_str());
		else
			mesh = new Mesh((std::string(Environment::GetRootPath()) + "/Models/suzanna_flat.obj").c_str());

		square->SetMesh(mesh);

		mainScene->AddChild(square);

		std::cout << "Creating obj" << square->GetGameObjectId() << "at: " << cubePositions[i].x << ", " << cubePositions[i].y << ", " << cubePositions[i].z << std::endl;
	}

	GameObject* plane = new GameObject(
		glm::vec3(0, -4, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(10, 10, 10),
		(std::string(Environment::GetRootPath()) + "/Shaders/ColorVert.glsl").c_str(),
		(std::string(Environment::GetRootPath()) + "/Shaders/ColorFrag.glsl").c_str());
		//(std::string(Environment::GetRootPath()) + "/Textures/bricks.jpg").c_str());

	Mesh* planeMesh = MeshCache::GetMesh((std::string(Environment::GetRootPath()) + "/Models/plane.obj").c_str());

	plane->SetMesh(planeMesh);
	mainScene->AddChild(plane);

	mRenderer = new Renderer();
	mRenderer->SetScreenHeight(1080);
	mRenderer->SetScreenWidth(1920);

	Camera* camera = new Camera(2.0f, 0.1f);
	camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	mRenderer->SetActiveCamera(camera);

	PointLight* lightSource1 = new PointLight(
		glm::vec3(1.0f),
		glm::vec3(0.0f),
		glm::vec3(0.5f),
		(std::string(Environment::GetRootPath()) + "/Shaders/LightSourceVert.glsl").c_str(),
		(std::string(Environment::GetRootPath()) + "/Shaders/LightSourceFrag.glsl").c_str(),
		glm::vec3(1.0f, 0.0f, 0.0f));

	Mesh* lightSourceMesh1 = MeshCache::GetMesh((std::string(Environment::GetRootPath()) + "/Models/cube_smooth.obj").c_str());
	lightSource1->SetMesh(lightSourceMesh1);
	mainScene->AddChild(lightSource1);
	mRenderer->AddLight(lightSource1);

	PointLight* lightSource3 = new PointLight(
		glm::vec3(1.0f),
		glm::vec3(0.0f),
		glm::vec3(0.5f),
		(std::string(Environment::GetRootPath()) + "/Shaders/LightSourceVert.glsl").c_str(),
		(std::string(Environment::GetRootPath()) + "/Shaders/LightSourceFrag.glsl").c_str(),
		glm::vec3(1.0f, 0.0f, 0.0f));

	Mesh* lightSourceMesh3 = MeshCache::GetMesh((std::string(Environment::GetRootPath()) + "/Models/cube_smooth.obj").c_str());
	lightSource3->SetMesh(lightSourceMesh3);
	mainScene->AddChild(lightSource3);
	mRenderer->AddLight(lightSource3);

	PointLight* lightSource4 = new PointLight(
		glm::vec3(1.0f),
		glm::vec3(0.0f),
		glm::vec3(0.5f),
		(std::string(Environment::GetRootPath()) + "/Shaders/LightSourceVert.glsl").c_str(),
		(std::string(Environment::GetRootPath()) + "/Shaders/LightSourceFrag.glsl").c_str(),
		glm::vec3(1.0f, 0.0f, 0.0f));

	Mesh* lightSourceMesh4 = MeshCache::GetMesh((std::string(Environment::GetRootPath()) + "/Models/cube_smooth.obj").c_str());
	lightSource4->SetMesh(lightSourceMesh4);
	mainScene->AddChild(lightSource4);
	mRenderer->AddLight(lightSource4);

	SpotLight* lightSource2 = new SpotLight(
		glm::vec3(-3.0f),
		glm::vec3(0.0f),
		glm::vec3(0.5f),
		(std::string(Environment::GetRootPath()) + "/Shaders/LightSourceVert.glsl").c_str(),
		(std::string(Environment::GetRootPath()) + "/Shaders/LightSourceFrag.glsl").c_str(),
		glm::vec3(0.0f, 0.0f, 1.0f));

	Mesh* lightSourceMesh2 = MeshCache::GetMesh((std::string(Environment::GetRootPath()) + "/Models/cube_smooth.obj").c_str());
	lightSource2->SetMesh(lightSourceMesh2);
	mainScene->AddChild(lightSource2);
	mRenderer->AddLight(lightSource2);

	std::cout << "Engine initialized." << std::endl << std::endl;
}

void Game::AddScene(Scene* scene) {
	mScenes.push_back(scene);
}

void Game::RemoveScene(unsigned int sceneId) {
	for (auto iterator = mScenes.begin(); iterator != mScenes.end(); iterator++) {
		if ((*iterator)->GetSceneId() == sceneId) {
			mScenes.erase(iterator); 
			delete(*iterator);
			return;
		}
	}
}

void Game::InitializeGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void Game::InitializeWindow() {
	Game::mWindow = glfwCreateWindow(1920, 1080, "Lux Mechanicus", NULL, NULL);
	if (mWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(mWindow);

	glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
}

void Game::InitializeGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	glViewport(0, 0, 1920, 1080);
}

void Game::RenderLoop() {
	while (!glfwWindowShouldClose(mWindow))
	{
		CalculateDeltaTime();
		ProcessInput();
		
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
		
		mRenderer->SetProjectionMatrix(projectionMatrix);
		mRenderer->Render(mScenes);

		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
	glfwTerminate();
}

void Game::ProcessInput() {
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(mWindow, true);

	Camera* camera = mRenderer->GetActiveCamera();

	glm::vec3 direction = glm::vec3(0.0f);

	if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
		direction.z -= 1;
	if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
		direction.z += 1;
	if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
		direction.x -= 1;
	if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
		direction.x += 1;

	//std::cout << direction.x << ";" << direction.y << ";" << direction.z << std::endl;
	//std::cout << mDeltaTime << std::endl;

	camera->ProcessMovementInput(direction, mDeltaTime);
}

Renderer* Game::GetRenderer() const{
	return mRenderer;
}

void Game::CalculateDeltaTime() {
	float currentFrameTime = static_cast<float>(glfwGetTime());
	mDeltaTime = currentFrameTime - mLastFrameTime;
	mLastFrameTime = currentFrameTime;

	mFrameCounter++;

	if (currentFrameTime - mLastTimerCounter >= 1.0) { 
		std::cout << "FPS: " << mFrameCounter << std::endl;
		mFrameCounter = 0; 
		mLastTimerCounter = currentFrameTime;
	}
}

void Game::MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	float horizontalPosition = static_cast<float>(xposIn);
	float verticalPosition = static_cast<float>(yposIn);
	float horizontalOffset;
	float verticalOffset;

	if (mLastXMousePos == 0 && mLastYMousePos == 0)
	{
		horizontalOffset = 0.0f;
		verticalOffset = 0.0f;
	}
	else {
	horizontalOffset = horizontalPosition - mLastXMousePos;
	verticalOffset = mLastYMousePos - verticalPosition;
	}

	mLastXMousePos = horizontalPosition;
	mLastYMousePos = verticalPosition;
	
	Camera* camera = mRenderer->GetActiveCamera();
	camera->ProcessRotationInput(horizontalOffset, verticalOffset);
}

void Game::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	mRenderer->SetScreenWidth(width);
	mRenderer->SetScreenHeight(height);
}