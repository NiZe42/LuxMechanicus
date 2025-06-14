#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(const std::vector<RenderTextureType> &outputRenderTextures) {
	Initialize(outputRenderTextures);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &FBOId);
}

void FrameBuffer::Initialize(const std::vector<RenderTextureType> &outputRenderTextures) {
    int outputRenderTexturesCount = outputRenderTextures.size();
    
    glGenFramebuffers(1, &FBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOId);
     
    for (int i = 0; i < outputRenderTexturesCount; i++) {
        unsigned int renderTextureId;

        glGenTextures(1, &renderTextureId);
        glBindTexture(GL_TEXTURE_2D, renderTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1920, 1080, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        attachedRenderTextures[outputRenderTextures[i]] = renderTextureId;

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, renderTextureId, 0);
    }

    glGenRenderbuffers(1, &RBODepthId);
    glBindRenderbuffer(GL_RENDERBUFFER, RBODepthId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);

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

