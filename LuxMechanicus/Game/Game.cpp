#include "Game.h"
#include <iostream>
#include <filesystem>

RenderProcessor* Game::renderProcessor = nullptr;
float Game::mLastXMousePos = 0;
float Game::mLastYMousePos = 0;

Game* Game::Instance = nullptr;

Game::Game() {
	if (Instance)
		return;

	Instance = this;

	Initialize();
}

Game::~Game() {
	delete renderProcessor;

	if (profiler)
		delete profiler;

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
	glfwSetKeyCallback(mWindow, WriteToCsvCallback);

	mLastFrameTime = static_cast<float>(glfwGetTime());
	mFrameCounter = 0;

	profiler = new Profiler();

	renderProcessor = new RenderProcessor();
	renderProcessor->SetScreenHeight(1080);
	renderProcessor->SetScreenWidth(1920);

	Scene* mainScene = new DifferentMeshDifferentMaterialsScene();
	mainScene->Initialize();
	AddScene(mainScene);

	renderProcessor->SetActiveCamera(mainScene->GetActiveCamera());
	for (int i = 0; i < mainScene->lightSources.size(); i++)
	{
		renderProcessor->AddLight(mainScene->lightSources[i]);
	}

	std::cout << "Engine initialized." << std::endl << std::endl;
	isInitialized = true;
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
		std::cerr << "Failed to initialize GLAD Loader" << std::endl;
		return;
	}
	if (!gladLoadGL()) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderer: " << renderer << "\n";
	std::cout << "OpenGL version: " << version << "\n";

	glViewport(0, 0, 1920, 1080);
}

void Game::RenderLoop() {
	renderProcessor->PrepareStaticInfo();
	while (!glfwWindowShouldClose(mWindow))
	{
		if (!isInitialized)
			continue;

		CalculateDeltaTime();
		ProcessInput();
		
		profiler->BeginFrame();

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		renderProcessor->Render(mScenes);

		profiler->EndFrame();

		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
	glfwTerminate();
}

void Game::ProcessInput() {
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(mWindow, true);

	Camera* camera = renderProcessor->GetActiveCamera();

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

RenderProcessor* Game::GetRenderer() const{
	return renderProcessor;
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
	} else {
		horizontalOffset = horizontalPosition - mLastXMousePos;
		verticalOffset = mLastYMousePos - verticalPosition;
	}

	mLastXMousePos = horizontalPosition;
	mLastYMousePos = verticalPosition;
	
	Camera* camera = renderProcessor->GetActiveCamera();
	camera->ProcessRotationInput(horizontalOffset, verticalOffset);
}

void Game::WriteToCsvCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		std::cout << "pressed" << std::endl;

		auto now = std::chrono::system_clock::now();
		std::time_t nowC = std::chrono::system_clock::to_time_t(now);

		std::tm localTime;
		localtime_s(&localTime, &nowC);  

		std::ostringstream filename;
		filename << "Logs/profiler_log_"
			<< std::put_time(&localTime, "%d-%m-%Y-%H-%M-%S")
			<< ".txt";

		if (Instance && Instance->profiler)
		{
			Instance->profiler->WriteAllToCSV(filename.str());
		}
	}
}

void Game::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	renderProcessor->SetScreenWidth(width);
	renderProcessor->SetScreenHeight(height);
}