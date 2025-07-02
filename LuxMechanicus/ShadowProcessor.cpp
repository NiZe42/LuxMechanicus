#include "ShadowProcessor.h"

ShadowProcessor::ShadowProcessor() {

}

ShadowProcessor::~ShadowProcessor() {

}

void ShadowProcessor::AllocateShadowAtlases(std::vector<LightData> shadowCasters) {
    shadowAtlases.clear();

    float shadowAtlasNumber = shadowCasters.size() / (float)(SHADOWS_PER_ATLAS);
    
    if (shadowAtlasNumber > floorf(shadowAtlasNumber))
        shadowAtlasNumber = ceilf(shadowAtlasNumber);

    for (int i = 0; i < (int)shadowAtlasNumber; i++) {
        
        ShadowAtlas newAtlas;
        glGenTextures(1, &newAtlas.textureID);
        glBindTexture(GL_TEXTURE_2D, newAtlas.textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, ATLAS_RES, ATLAS_RES, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        shadowAtlases.push_back(newAtlas);
    }
}
