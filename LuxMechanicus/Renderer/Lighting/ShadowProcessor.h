#pragma once
#include "..\..\Utils\GLIncludes.h"
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include "LightData.h"
#include "LightSource.h"
#include "..\FrameBuffer\FrameBuffer.h"
#include "..\..\Game\Scenes\Scene.h"
#include "..\..\Utils\Environment.h"
#include "../Batching/Batcher.h"

class ShadowProcessor {
public:
    static const int SHADOW_MAP_RES = 1024;
    static const int ATLAS_RES = 4096;
    static const int SHADOWS_PER_ATLAS = (ATLAS_RES / SHADOW_MAP_RES) * (ATLAS_RES / SHADOW_MAP_RES);

    ShadowProcessor();
    ~ShadowProcessor();

    void ShadowPass(
        const std::vector<Scene*>& scenesToRender, 
        Batcher* batcher);
    void RenderScene(Scene* scene);
    void RenderGameObject(GameObject* object);

    void AllocateShadowAtlases(const std::vector<LightData>& lights);

    void UploadShadowCastersToGPU();
    void BindSSBO(unsigned int binding);

    void BindAtlasFramebuffer();
    void UnbindAtlasFramebuffer();
    
    FrameBuffer* GetFrameBuffer() const;
    glm::ivec2 GetAtlasSlot(int shadowIndex) const;

private:

    unsigned int SSBOId = 0;
    std::unordered_map<int, ShadowCasterData> currentShadowCasters;
    FrameBuffer* shadowPassFrameBuffer = nullptr;
    Shader* shadowPassShader = nullptr;
};
