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
#include "../../Renderer/Lighting/LightSource.h"

class Scene : public HierarchyObject
{
public:
	Scene();
	virtual ~Scene();

	unsigned int GetSceneId() const;

	virtual void Initialize();

	std::unordered_map<unsigned int, GameObject> sceneGameObjects;
	std::vector<LightSource*> lightSources;
	Camera* GetActiveCamera();

protected:
	unsigned int mSceneId;
	static unsigned int nextSceneId;

	Camera* activeCamera;
};

