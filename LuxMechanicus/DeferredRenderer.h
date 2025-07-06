#pragma once

#include "GLIncludes.h"
#include <vector>
#include "Camera.h"
#include "Scene.h"
#include "Shader.h"
#include "LightProcessor.h"
#include "Mesh.h"
#include <iostream>
#include "FrameBuffer.h"
#include "Environment.h"

class DeferredRenderer {
public:
    DeferredRenderer();
    ~DeferredRenderer();

    void Initialize(unsigned int width, unsigned int height);

    void GeometryPass(const std::vector<Scene*>& scenesToRender, const Camera& camera);
    void LightingPass(const Camera& camera, unsigned int DepthLayeredTextureArrayId);

    FrameBuffer* GetGFrameBuffer();

    void RenderFullscreenQuad();
private:
    void InitializeQuad();
    

    void RenderGameObject(GameObject* object);

    FrameBuffer* gFrameBuffer;

    Shader* geometryShader;
    Shader* lightingShader;

    unsigned int quadVAOId, quadVBOId;
};
