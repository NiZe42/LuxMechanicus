#pragma once

#include "GLIncludes.h"
#include <vector>
#include "Camera.h"
#include "Scene.h"
#include "Shader.h"
#include "LightProcessor.h"

class ForwardRenderer {
public:
    ForwardRenderer();
    ~ForwardRenderer();

    void Initialize();

    void RenderForwardGameObjectList(const Camera& camera);
    
    void AddGameObject(GameObject* gameObject);

private:
    std::vector<GameObject*> gameObjectList;
};
