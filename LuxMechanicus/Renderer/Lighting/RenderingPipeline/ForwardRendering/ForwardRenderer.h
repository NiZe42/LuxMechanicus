#pragma once
#include "..\..\..\..\Utils\GLIncludes.h"
#include <vector>
#include "..\..\..\..\Game\Camera\Camera.h"
#include "..\..\..\..\Game\GameObject\GameObject.h"
#include "..\..\..\Shader\Shader.h"
#include "..\..\LightProcessor.h"

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
