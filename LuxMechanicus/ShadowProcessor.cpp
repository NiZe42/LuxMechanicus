#include "ShadowProcessor.h"
#include <cmath>

ShadowProcessor::ShadowProcessor() {}

ShadowProcessor::~ShadowProcessor() {
    for (auto& atlas : shadowAtlases) {
        glDeleteTextures(1, &atlas.textureID);
    }

    for (auto& fb : atlasFramebuffers) {
        delete fb;
    }
}

void ShadowProcessor::AllocateShadowAtlases(const std::vector<LightSource>& shadowCasters) {
    for (auto& atlas : shadowAtlases) {
        glDeleteTextures(1, &atlas.textureID);
    }

    for (auto& fb : atlasFramebuffers) {
        delete fb;
    }
    atlasFramebuffers.clear();

    shadowAtlases.clear();
    currentShadowCasters.clear();

    float numAtlases = shadowCasters.size() / (float)SHADOWS_PER_ATLAS;
    if (numAtlases > floorf(numAtlases))
        numAtlases = ceilf(numAtlases);

    if (numAtlases > MAX_SHADOW_ATLASES) {
        std::cerr << "Exceeded maximum shadow atlas count (8). Clamping." << std::endl;
        numAtlases = MAX_SHADOW_ATLASES;
    }

    for (int i = 0; i < (int)numAtlases; i++) {
        ShadowAtlas newAtlas;
        glGenTextures(1, &newAtlas.textureID);
        glBindTexture(GL_TEXTURE_2D, newAtlas.textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, ATLAS_RES, ATLAS_RES, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        shadowAtlases.push_back(newAtlas);
    }

    for (int i = 0; i < shadowCasters.size(); i++) {
        LightSource light = shadowCasters[i];
        light.SetshadowMapIndex(i);
        currentShadowCasters.emplace(i, light);
    }
}

// Texture from 8 to 16 are ShadowAtlases
void ShadowProcessor::BindShadowAtlases() {
    for (int i = 0; i < shadowAtlases.size(); i++) {
        glActiveTexture(GL_TEXTURE8 + i);
        glBindTexture(GL_TEXTURE_2D, shadowAtlases[i].textureID);
    }
}

void ShadowProcessor::UnbindShadowAtlases() {
    for (int i = 0; i < shadowAtlases.size(); i++) {
        glActiveTexture(GL_TEXTURE8 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

int ShadowProcessor::GetAtlasTextureForLight(const LightSource& light) const {
    if (!light.GetCastShadows()) {
        std::cerr << "The passed light cannot cast shadows." << std::endl;
        return -1;
    }

    auto it = currentShadowCasters.find(light.GetshadowMapIndex());
    if (it != currentShadowCasters.end()) {
        int index = it->second.GetshadowMapIndex();
        int atlasIndex = index / SHADOWS_PER_ATLAS;
        return shadowAtlases[atlasIndex].textureID;
    }
    std::cerr << "The passed light is not in Shadow Atlases" << std::endl;
    return -1;
}

glm::ivec2 ShadowProcessor::GetAtlasSlot(int shadowIndex) const {
    int x = (shadowIndex % SHADOWS_PER_ATLAS) % SHADOWS_PER_ATLAS;
    int y = (shadowIndex % SHADOWS_PER_ATLAS) / SHADOWS_PER_ATLAS;
    return glm::ivec2(x, y);
}
