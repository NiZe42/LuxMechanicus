#include "RenderProcessor.h"
#include <iostream>

Camera* RenderProcessor::mActiveCamera = nullptr;

RenderProcessor::RenderProcessor() {
    glEnable(GL_DEPTH_TEST);

    postProcessor = new PostProcessor();
    lightProcessor = new LightProcessor();
    deferredRenderer = new DeferredRenderer();
    forwardRenderer = new ForwardRenderer();
}

RenderProcessor::~RenderProcessor() {
    if (mActiveCamera)
        delete mActiveCamera;

    if (postProcessor)
        delete postProcessor;

    if (lightProcessor)
        delete lightProcessor;
}

void RenderProcessor::Render(const std::vector<Scene*>& scenesToRender) {

    deferredRenderer->GeometryPass(scenesToRender, *mActiveCamera);

    postProcessor->BindFirstFrameBuffer();
   
    lightProcessor->UploadToGPU();
    lightProcessor->BindSSBO(1);

    deferredRenderer->LightingPass(*mActiveCamera);

    FrameBuffer* gFrameBuffer = deferredRenderer->GetGFrameBuffer();
    FrameBuffer* firstFrameBuffer = postProcessor->GetFirstFrameBuffer();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, gFrameBuffer->GetFrameBufferId());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, firstFrameBuffer->GetFrameBufferId());
    glBlitFramebuffer(
        0,
        0,
        mScreenWidth,
        mScreenHeight,
        0,
        0,
        mScreenWidth,
        mScreenHeight,
        GL_DEPTH_BUFFER_BIT,
        GL_NEAREST
    );

    forwardRenderer->RenderForwardGameObjectList(*mActiveCamera);

    postProcessor->UnbindFirstFrameBuffer();
    
    postProcessor->ApplyEffectsAndRender();
}

void RenderProcessor::SetActiveCamera(Camera* camera) {
    if(mActiveCamera != nullptr)
        delete mActiveCamera;
    mActiveCamera = camera;
}

Camera* RenderProcessor::GetActiveCamera() {
    return mActiveCamera;
}

// All Lights automatically will be rendered by forward Renderer.
void RenderProcessor::AddLight(LightSource* lightSource) {
    lightProcessor->AddLight(lightSource->GetLightData());
    forwardRenderer->AddGameObject(lightSource);
}

void RenderProcessor::SetScreenHeight(int height) {
    mScreenHeight = height;
}

void RenderProcessor::SetScreenWidth(int width) {
    mScreenWidth = width;
}