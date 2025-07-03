#include "FrameBuffer.h"

#pragma region Framebuffer 

FrameBuffer::FrameBuffer() {

}

FrameBuffer::FrameBuffer(
    const std::vector<RenderTextureType>& outputRenderTextures,
    int width,
    int height) {
    Initialize(outputRenderTextures, width, height);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &FBOId);
}

void FrameBuffer::Initialize(
    const std::vector<RenderTextureType>& outputRenderTextures,
    int screenWidth,
    int screenHeight) {

    int outputRenderTexturesCount = outputRenderTextures.size();

    glGenFramebuffers(1, &FBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOId);

    for (int i = 0; i < outputRenderTexturesCount; i++) {
        unsigned int renderTextureId;

        glGenTextures(1, &renderTextureId);
        glBindTexture(GL_TEXTURE_2D, renderTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        attachedRenderTextures[outputRenderTextures[i]] = renderTextureId;

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, renderTextureId, 0);
    }

    glGenRenderbuffers(1, &RBODepthId);
    glBindRenderbuffer(GL_RENDERBUFFER, RBODepthId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBODepthId);

    std::vector<unsigned int> attachments;

    for (int i = 0; i < outputRenderTexturesCount; i++) {
        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glDrawBuffers(outputRenderTexturesCount, attachments.data());

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is incomplete!" << std::endl;
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        switch (status) {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cerr << "Framebuffer incomplete attachment" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cerr << "Framebuffer missing attachment" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cerr << "Framebuffer incomplete draw buffer" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cerr << "Framebuffer incomplete read buffer" << std::endl;
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cerr << "Framebuffer unsupported" << std::endl;
            break;
        default:
            std::cerr << "Framebuffer error: " << status << std::endl;
        }
    }
}

void FrameBuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, FBOId);
}

void FrameBuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBuffer::GetRBODepthId() const {
    return RBODepthId;
}

unsigned int FrameBuffer::GetAttachedRenderTextureIdByType(RenderTextureType renderTextureType) const {
    if (attachedRenderTextures.size() == 0)
        std::cerr << "No attached Render Textures" << std::endl;
    return attachedRenderTextures.at(renderTextureType);
}

unsigned int FrameBuffer::GetFrameBufferId() const {
    return FBOId;
}

#pragma endregion

#pragma region Builder 

FrameBuffer::Builder::Builder() {
    framebuffer = FrameBuffer();
}

FrameBuffer::Builder& FrameBuffer::Builder::WithRenderTextures(
    const std::vector<RenderTextureType>& outputRenderTextures,
    int width,
    int height) {

    if (framebuffer.depthAttachmentType != DepthAttachmentType::NONE) {
        std::cerr << "Warning: Adding render textures after depth attachment." << std::endl;
    }

    framebuffer.screenWidth = width;
    framebuffer.screenHeight = height;

    glGenFramebuffers(1, &framebuffer.FBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.FBOId);

    for (int i = 0; i < outputRenderTextures.size(); ++i) {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        framebuffer.attachedRenderTextures[outputRenderTextures[i]] = textureID;

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureID, 0);
    }

    std::vector<GLenum> drawBuffers;
    for (int i = 0; i < outputRenderTextures.size(); ++i) {
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers(drawBuffers.size(), drawBuffers.data());

    return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::WithDepthRBO(int width, int height) {
    if (framebuffer.depthAttachmentType != DepthAttachmentType::NONE) {
        std::cerr << "Only one depth attachment is allowed." << std::endl;
        return *this;
    }

    framebuffer.depthAttachmentType = DepthAttachmentType::RENDERBUFFER;

    glGenRenderbuffers(1, &framebuffer.RBODepthId);
    glBindRenderbuffer(GL_RENDERBUFFER, framebuffer.RBODepthId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebuffer.RBODepthId);

    return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::WithDepthTexture(int width, int height) {
    if (framebuffer.depthAttachmentType != DepthAttachmentType::NONE) {
        std::cerr << "Only one depth attachment is allowed." << std::endl;
        return *this;
    }

    framebuffer.depthAttachmentType = DepthAttachmentType::TEXTURE_2D;

    GLuint depthTextureID;
    glGenTextures(1, &depthTextureID);
    glBindTexture(GL_TEXTURE_2D, depthTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    framebuffer.depthTextureID = depthTextureID;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0);

    return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::WithDepthTextureArray(int width, int height) {
    if (framebuffer.depthAttachmentType != DepthAttachmentType::NONE) {
        std::cerr << "Only one depth attachment is allowed." << std::endl;
        return *this;
    }

    framebuffer.depthAttachmentType = DepthAttachmentType::TEXTURE_2D_ARRAY_LAYER;

    GLuint depthArrayID;
    glGenTextures(1, &depthArrayID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthArrayID);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT24, width, height, 8, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    framebuffer.depthTextureArrayID = depthArrayID;
    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthArrayID, 0, 0);

    return *this;
}

FrameBuffer FrameBuffer::Builder::Build() {
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is incomplete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return framebuffer;
}

#pragma endregion