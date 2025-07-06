#include "FrameBuffer.h"

#pragma region Framebuffer 

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &FBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOId);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &FBOId);
}

void FrameBuffer::CheckFrameBuffer() const {
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

FrameBuffer::DepthAttachmentType FrameBuffer::GetDepthAttachmentType() const {
    return depthAttachmentType;
}

void FrameBuffer::SetDepthAttachmentType(FrameBuffer::DepthAttachmentType newDepthAttachmentType) {
    depthAttachmentType = newDepthAttachmentType;
}

unsigned int FrameBuffer::GetRBODepthId() const {
    if (GetDepthAttachmentType() != DepthAttachmentType::RENDERBUFFER) {
        std::cerr << "The FrameBuffer has a different type of depth attachment: " 
            << DepthAttachmentHelper::ToString(depthAttachmentType) 
            << ". You are requesting " 
            << DepthAttachmentHelper::ToString(DepthAttachmentType::RENDERBUFFER) 
            << "." << std::endl;
    }
    return RBODepthId;
}

unsigned int FrameBuffer::GetDepthTextureId() const {
    if (GetDepthAttachmentType() != DepthAttachmentType::TEXTURE_2D) {
        std::cerr << "The FrameBuffer has a different type of depth attachment: "
            << DepthAttachmentHelper::ToString(depthAttachmentType)
            << ". You are requesting "
            << DepthAttachmentHelper::ToString(DepthAttachmentType::TEXTURE_2D)
            << "." << std::endl;
    }
    return depthTextureId;
}

unsigned int FrameBuffer::GetDepthArrayId() const {
    if (GetDepthAttachmentType() != DepthAttachmentType::TEXTURE_2D_LAYERED_ARRAY) {
        std::cerr << "The FrameBuffer has a different type of depth attachment: "
            << DepthAttachmentHelper::ToString(depthAttachmentType)
            << ". You are requesting "
            << DepthAttachmentHelper::ToString(DepthAttachmentType::TEXTURE_2D_LAYERED_ARRAY)
            << "." << std::endl;
    }
    return depthArrayId;
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
    frameBuffer = new FrameBuffer();
}

FrameBuffer::Builder& FrameBuffer::Builder::WithRenderTextures(
    const std::vector<RenderTextureType>& outputRenderTextures,
    int width,
    int height) {

    int outputRenderTexturesCount = outputRenderTextures.size();

    if (frameBuffer->GetDepthAttachmentType() != DepthAttachmentType::NONE) {
        std::cerr << "Warning: Adding render textures after depth attachment." << std::endl;
    }

    for (int i = 0; i < outputRenderTexturesCount; i++) {
        unsigned int renderTextureId;

        glGenTextures(1, &renderTextureId);
        glBindTexture(GL_TEXTURE_2D, renderTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        frameBuffer->attachedRenderTextures[outputRenderTextures[i]] = renderTextureId;

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, renderTextureId, 0);
    }

    std::vector<unsigned int> attachments;

    for (int i = 0; i < outputRenderTexturesCount; i++) {
        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glDrawBuffers(outputRenderTexturesCount, attachments.data());

    return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::WithDepthRBO(int width, int height) {
    if (frameBuffer->GetDepthAttachmentType() != DepthAttachmentType::NONE) {
        std::cerr << "Only one depth attachment is allowed." << std::endl;
        return *this;
    }

    frameBuffer->SetDepthAttachmentType(DepthAttachmentType::RENDERBUFFER);

    glGenRenderbuffers(1, &frameBuffer->RBODepthId);
    glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer->RBODepthId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBuffer->RBODepthId);

    return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::WithDepthTexture(int width, int height) {
    if (frameBuffer->GetDepthAttachmentType() != DepthAttachmentType::NONE) {
        std::cerr << "Only one depth attachment is allowed." << std::endl;
        return *this;
    }

    frameBuffer->SetDepthAttachmentType(DepthAttachmentType::TEXTURE_2D);

    glGenTextures(1, &frameBuffer->depthTextureId);
    glBindTexture(GL_TEXTURE_2D, frameBuffer->depthTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, frameBuffer->depthTextureId, 0);

    return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::WithDepthTextureArray(int count, int width, int height) {
    if (frameBuffer->GetDepthAttachmentType() != DepthAttachmentType::NONE) {
        std::cerr << "Only one depth attachment is allowed." << std::endl;
        return *this;
    }
    // Will break if it is more than 2048 layers (on some machines).
    // TODO: Dynamically clamp the size.

    frameBuffer->SetDepthAttachmentType(DepthAttachmentType::TEXTURE_2D_LAYERED_ARRAY);
    std::cout << "count: " << count << std::endl;
    glGenTextures(1, &frameBuffer->depthArrayId);
    glBindTexture(GL_TEXTURE_2D_ARRAY, frameBuffer->depthArrayId);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16, width, height, count, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, frameBuffer->depthArrayId, 0);

    return *this;
}

FrameBuffer* FrameBuffer::Builder::Build() {
    frameBuffer->CheckFrameBuffer();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return frameBuffer;
}

#pragma endregion