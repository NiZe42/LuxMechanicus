#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "LightSource.h"
#include <iostream>
#include "Shader.h"
#include "Environment.h"
#include "PostProcessor.h"
#include "LightProcessor.h"
#include "ForwardRenderer.h"
#include "DeferredRenderer.h"
#include "ShadowProcessor.h"

class RenderProcessor {
public:
    RenderProcessor();
    ~RenderProcessor();

    void PrepareStaticInfo();

    void Render(const std::vector<Scene*>& scenesToRender);

    void SetActiveCamera(Camera* camera);
    static Camera* GetActiveCamera();

    void AddLight(LightSource* lightSource);

    void SetScreenWidth(int width);
    void SetScreenHeight(int height);

private:
    static Camera* mActiveCamera;

    int mScreenWidth, mScreenHeight;

    PostProcessor* postProcessor;
    ShadowProcessor* shadowProcessor;
    LightProcessor* lightProcessor;
    ForwardRenderer* forwardRenderer;
    DeferredRenderer* deferredRenderer;
};


