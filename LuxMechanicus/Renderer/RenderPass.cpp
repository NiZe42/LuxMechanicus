#include "RenderPass.h"

RenderPass::RenderPass() {

}

RenderPass::~RenderPass() {
    if (effectShader)
        delete effectShader;
    if (frameBuffer)
        delete frameBuffer;
}

void RenderPass::BindEffectShader() const {
    if (effectShader)
        effectShader->Bind();
}

void RenderPass::UnbindEffectShader() const {
    if (effectShader)
        effectShader->Unbind();
}

void RenderPass::BindFramebuffer() const {
    if (frameBuffer)
        frameBuffer->Bind();
}

void RenderPass::UnbindFramebuffer() const {
    if (frameBuffer)
        frameBuffer->Unbind();
}

// Got to bind effectShader before and unbind after.
void RenderPass::SetUniforms(RenderTexturesPool* renderTexturesPool) const {
    
}

void RenderPass::Apply(const unsigned int &quadVAOId, RenderTexturesPool* renderTexturesPool) {
    
}

void RenderPass::RenderQuad(const unsigned int &quadVAOId) const {
    glBindVertexArray(quadVAOId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
