#include "Scene.h"

unsigned int Scene::nextSceneId = 0;

Scene::Scene() {
	mSceneId = nextSceneId++;

	//Initialize();
}

Scene::~Scene() {
	
	if (activeCamera)
		delete activeCamera;

	while (!lightSources.empty()) 
	{
		auto& last = lightSources.back();
		delete last;
		lightSources.pop_back();
	}

}

void Scene::Initialize() {

}

unsigned int Scene::GetSceneId() const {
	return mSceneId;
}

Camera* Scene::GetActiveCamera()
{
	return activeCamera;
}