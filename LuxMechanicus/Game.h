#pragma once
#include "GLIncludes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "Scene.h"
#include "RenderProcessor.h"
#include "Camera.h"
#include "Singleton.h"
#include "MeshCache.h"
#include "Environment.h"

class Game {
public: 
	Game();
	~Game();

	void Initialize();
	void RenderLoop();

	void AddScene(Scene* scene);
	void RemoveScene(unsigned int sceneId);

	RenderProcessor* GetRenderer() const;

	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double xposIn, double yposIn);

private:
	GLFWwindow* mWindow;

	static RenderProcessor* rendererProcessor;
	std::vector<Scene*> mScenes;

	float mLastFrameTime;
	float mDeltaTime;

	int mFrameCounter;
	float mLastTimerCounter;

	static float mLastXMousePos;
	static float mLastYMousePos;

	void InitializeGLFW();
	void InitializeWindow();
	void InitializeGLAD();
	
	void CalculateDeltaTime();

	void ProcessInput();
};

