#pragma once
#include <glad/glad.h> 
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "GameObject.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include "HierarchyObject.h"
#include "Camera.h"

class Scene : public HierarchyObject
{
public:
	Scene();
	~Scene();

	void Render();

	unsigned int GetSceneId() const;

	std::unordered_map<unsigned int, GameObject> sceneGameObjects;

protected:
	unsigned int mSceneId;
	static unsigned int nextSceneId;

	Shader* mShader;
	Texture* mTexture;
	Camera* mActiveCamera;

	void RenderTriangle();
};

