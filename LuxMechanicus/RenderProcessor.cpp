#include "RenderProcessor.h"
#include <iostream>

Camera* RenderProcessor::mActiveCamera = nullptr;

RenderProcessor::RenderProcessor() {
    glEnable(GL_DEPTH_TEST);

    postProcessor = new PostProcessor();
    shadowProcessor = new ShadowProcessor();
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

    if (shadowProcessor)
        delete shadowProcessor;
}

void RenderProcessor::PrepareStaticInfo() {
    lightProcessor->UploadToGPU();
    lightProcessor->BindSSBO(1);

    shadowProcessor->AllocateShadowAtlases(lightProcessor->GetLights());
    shadowProcessor->UploadShadowCastersToGPU();
    shadowProcessor->BindSSBO(2);
}

void RenderProcessor::Render(const std::vector<Scene*>& scenesToRender) {
    shadowProcessor->ShadowPass(scenesToRender);

    /*glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT || GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, 1024, 1024);

    Shader* shader = new Shader((std::string(Environment::GetRootPath()) + "/Shaders/TestQuadVert.glsl").c_str(),
        (std::string(Environment::GetRootPath()) + "/Shaders/TestQuadFrag.glsl").c_str());
    shader->Bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, shadowProcessor->GetFrameBuffer()->GetDepthArrayId());
   
    shader->SetUniformInt("depthMap", 0);

    deferredRenderer->RenderFullscreenQuad();
    delete shader;
    return;*/

    deferredRenderer->GeometryPass(scenesToRender, *mActiveCamera);

    postProcessor->BindFirstFrameBuffer();

    unsigned int depthArrayid = shadowProcessor->GetFrameBuffer()->GetDepthArrayId();
    deferredRenderer->LightingPass(*mActiveCamera, depthArrayid);

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