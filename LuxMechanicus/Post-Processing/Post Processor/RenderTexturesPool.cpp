#include "RenderTexturesPool.h"

RenderTexturesPool::RenderTexturesPool() {

}

RenderTexturesPool::~RenderTexturesPool() {
    pool.clear();
}

// Format and dimensions of both textures should be the same, i.e. GL_TEXTURE_2D and 1920x1080.
void RenderTexturesPool::SaveRenderTextureById(RenderTextureType renderTextureType, unsigned int renderTextureId) {
    auto element = pool.find(renderTextureType);
    if (element != pool.end()) {
        glCopyImageSubData(
            renderTextureId, GL_TEXTURE_2D, 0, 0, 0, 0,
            element->second, GL_TEXTURE_2D, 0, 0, 0, 0,
            1920, 1080, 1
        );
        return;
    }
    
    unsigned int textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1920, 1080, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (glCopyImageSubData) {
        glCopyImageSubData(
            renderTextureId, GL_TEXTURE_2D, 0, 0, 0, 0,
            textureId, GL_TEXTURE_2D, 0, 0, 0, 0,
            1920, 1080, 1
        );
    }
    else {
        std::cerr << "glCopyImageSubData is not loaded. OpenGL version too low or loader failed." << std::endl;
    }


    pool[renderTextureType] = textureId;
}


unsigned int RenderTexturesPool::GetRenderTextureIdOfType(RenderTextureType renderTextureType) {
    if (renderTextureType == RenderTextureType::NONE) {
        std::cerr << "Invalid request for render texture" << std::endl;
        return 0;
    }

    auto element = pool.find(renderTextureType);
    if (element != pool.end()) {
        return element->second;
    }

    std::cerr << "There are no render textures with requested type" << std::endl;
    return 0;
}


