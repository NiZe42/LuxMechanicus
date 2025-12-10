#pragma once
#include "..\Utils\GLIncludes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "..\Game\Scenes\Scene.h"
#include "..\Game\Scenes\SceneList\SameMeshSameMaterialScene.h"
#include "..\Renderer\RenderProcessor\RenderProcessor.h"
#include "..\Game\Camera\Camera.h"
#include "..\Utils\Singleton.h"
#include "..\Renderer\Mesh\MeshCache.h"
#include "..\Utils\Environment.h"
#include "..\Profiler\Profiler.h"

class Game {
public: 
	Game();
	~Game();

	static Game* Instance;

	void Initialize();
	void RenderLoop();

	void AddScene(Scene* scene);
	void RemoveScene(unsigned int sceneId);

	RenderProcessor* GetRenderer() const;

	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
	static void WriteToCsvCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	GLFWwindow* mWindow;

	Profiler* profiler;

	static RenderProcessor* renderProcessor;
	std::vector<Scene*> mScenes;

	float mLastFrameTime;
	float mDeltaTime;

	int mFrameCounter;
	float mLastTimerCounter;

	static float mLastXMousePos;
	static float mLastYMousePos;

	bool isInitialized = false;

	void InitializeGLFW();
	void InitializeWindow();
	void InitializeGLAD();
	
	void CalculateDeltaTime();

	void ProcessInput();
};

