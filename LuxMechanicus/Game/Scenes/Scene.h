#pragma once
#include "..\..\Utils\GLIncludes.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "..\GameObject\GameObject.h"
#include "..\..\Renderer\Shader\Shader.h"
#include "..\..\Renderer\Material\Texture.h"
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include "HierarchyObject.h"
#include "..\Camera\Camera.h"

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

	Shader* pShader;
	Texture* pTexture;
	Camera* mActiveCamera;

	void RenderTriangle();
};

