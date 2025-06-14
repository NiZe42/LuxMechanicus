#include "PostProcessor.h"

PostProcessor::PostProcessor() {
    Initialize();

    RenderPass* hdrEffect = new HDRRenderPass();
    AddEffect(hdrEffect);

    RenderPass* bloomEffect = new BloomEffect();
    AddEffect(bloomEffect);
}

PostProcessor::~PostProcessor() {
    if (emptyQuadShader)
        delete(emptyQuadShader);

    if (renderTexturesPool)
        delete(renderTexturesPool);

    if (firstFrameBuffer)
        delete(firstFrameBuffer);

    if (renderPassStack.empty())
        return;

    for (int i = 0; i < renderPassStack.size(); i++) {
        delete(renderPassStack[i]);
    }
    renderPassStack.clear();
}

void PostProcessor::ApplyEffectsAndRender() { 

    if (!renderPassStack.empty()) {
        renderTexturesPool->SaveRenderTextureById(RenderTextureType::COLOR, firstFrameBuffer->GetAttachedRenderTextureIdByType(RenderTextureType::COLOR));

	    for (int i = 0; i < renderPassStack.size(); i++) {
            renderPassStack[i]->Apply(quadVAOId, renderTexturesPool);
	    }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    emptyQuadShader->Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTexturesPool->GetRenderTextureIdOfType(RenderTextureType::COLOR));
    emptyQuadShader->SetUniformInt("ColorMap", 0);

    RenderQuad();

    emptyQuadShader->Unbind();
}

void PostProcessor::BindFirstFrameBuffer() {
    firstFrameBuffer->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessor::UnbindFirstFrameBuffer() {
    firstFrameBuffer->Unbind();
}

void PostProcessor::Initialize() {
    renderTexturesPool = new RenderTexturesPool();

    emptyQuadShader = new Shader(
        (std::string(Environment::GetRootPath()) + "/Shaders/EmptyQuadVert.glsl").c_str(),
        (std::string(Environment::GetRootPath()) + "/Shaders/EmptyQuadFrag.glsl").c_str());

    firstFrameBuffer = new FrameBuffer(std::vector<RenderTextureType>{RenderTextureType::COLOR});

    InitializeQuad();
}

void PostProcessor::InitializeQuad() {
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &quadVAOId);

    glGenBuffers(1, &quadVBOId);
    glBindVertexArray(quadVAOId);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBOId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void PostProcessor::AddEffect(RenderPass* renderPass) {
    renderPassStack.push_back(renderPass);
}

void PostProcessor::RenderQuad() const {
    glBindVertexArray(quadVAOId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

FrameBuffer* PostProcessor::GetFirstFrameBuffer() {
    return firstFrameBuffer;
}