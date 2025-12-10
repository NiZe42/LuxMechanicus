#pragma once
#include "..\..\..\..\Utils\GLIncludes.h"
#include <vector>
#include "..\..\..\..\Game\Camera\Camera.h"
#include "..\..\..\..\Game\Scenes\Scene.h"
#include "..\..\..\..\Renderer\Shader\Shader.h"
#include "..\..\..\..\Renderer\Lighting\LightProcessor.h"
#include "..\..\..\..\Renderer\Mesh\Mesh.h"
#include <iostream>
#include "..\..\..\..\Renderer\FrameBuffer\FrameBuffer.h"
#include "..\..\..\..\Utils\Environment.h"
#include "..\..\..\..\Profiler\Profiler.h"
#include "../../../Batching/MeshVaoProcessor.h"

class DeferredRenderer {
public:
    DeferredRenderer();
    ~DeferredRenderer();

    void Initialize(unsigned int width, unsigned int height);

    void GeometryPass(
        const std::vector<Scene*>& scenesToRender, 
        const Camera& camera, 
        MeshVaoProcessor* meshVaoProcessor);

    void LightingPass(const Camera& camera, unsigned int DepthLayeredTextureArrayId);

    FrameBuffer* GetGFrameBuffer();

    void RenderFullscreenQuad();
private:
    void InitializeQuad();
    
    void RenderScene(Scene* scene);
    void RenderGameObject(GameObject* object);

    FrameBuffer* gFrameBuffer;

    Shader* geometryShader;
    Shader* lightingShader;

    unsigned int quadVAOId, quadVBOId;
};
