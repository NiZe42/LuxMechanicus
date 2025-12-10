#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "..\..\Game\Scenes\Scene.h"
#include "..\..\Game\GameObject\GameObject.h"
#include "..\..\Game\Camera\Camera.h"
#include "..\Lighting\LightSource.h"
#include <iostream>
#include "..\Shader\Shader.h"
#include "..\..\Utils\Environment.h"
#include "..\..\Post-Processing\PostProcessor\PostProcessor.h"
#include "..\Lighting\LightProcessor.h"
#include "..\Lighting\RenderingPipeline\ForwardRendering\ForwardRenderer.h"
#include "..\Lighting\RenderingPipeline\DeferredRendering\DeferredRenderer.h"
#include "..\Lighting\ShadowProcessor.h"
#include "../Batching/MeshVaoProcessor.h"

class RenderProcessor {
public:
    RenderProcessor();
    ~RenderProcessor();

    void PrepareStaticInfo(Scene* scene);

    void Render(const std::vector<Scene*>& scenesToRender);

    void SetActiveCamera(Camera* camera);
    static Camera* GetActiveCamera();

    void AddLight(LightSource* lightSource);

    void SetScreenWidth(int width);
    void SetScreenHeight(int height);

private:
    static Camera* mActiveCamera;

    int mScreenWidth, mScreenHeight;

    MeshVaoProcessor* meshVaoProcessor;
    PostProcessor* postProcessor;
    ShadowProcessor* shadowProcessor;
    LightProcessor* lightProcessor;
    ForwardRenderer* forwardRenderer;
    DeferredRenderer* deferredRenderer;
};


