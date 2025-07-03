#pragma once
#include "GLIncludes.h"
#include <glm/glm.hpp>
#include <vector>
#include <ostream>
#include <unordered_map>
#include "LightData.h"
#include "LightSource.h"
#include "FrameBuffer.h"

class ShadowProcessor {
public:
    const static int SHADOW_MAP_RES = 1024;
    const static int ATLAS_RES = 4096;
    const static int MAX_SHADOW_ATLASES = 8;
    const static int SHADOWS_PER_ATLAS = (ATLAS_RES / SHADOW_MAP_RES) * (ATLAS_RES / SHADOW_MAP_RES);

    ShadowProcessor();
    ~ShadowProcessor();

    void AllocateShadowAtlases(const std::vector<LightSource>& shadowCasters);

    void BindAtlasFramebuffer(int atlasIndex);
    void UnbindAtlasFramebuffer();

    void BindShadowAtlases();
    void UnbindShadowAtlases();

    int GetAtlasTextureForLight(const LightSource& light) const;
    glm::ivec2 GetAtlasSlot(int shadowIndex) const;

private:
    struct ShadowAtlas {
        GLuint textureID;
    };

    std::vector<ShadowAtlas> shadowAtlases;
    std::vector<FrameBuffer*> atlasFramebuffers;
    std::unordered_map<int, LightSource> currentShadowCasters;
};
