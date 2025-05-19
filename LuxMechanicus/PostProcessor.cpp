#include "PostProcessor.h"

PostProcessor::PostProcessor() {
    Initialize();

    PostProcessingEffect* hdrEffect = new HDREffect();
    hdrEffect->Initialize();

    AddEffect(hdrEffect);
}

PostProcessor::~PostProcessor() {
    if (emptyQuadShader)
        delete(emptyQuadShader);

    if (frameBuffer1)
        delete(frameBuffer1);

    if (frameBuffer2)
        delete(frameBuffer2);

    if (effectsStack.empty())
        return;

    for (int i = 0; i < effectsStack.size(); i++) {
        delete(effectsStack[i]);
    }
}

void PostProcessor::ApplyEffectsAndRender() { 

	if (effectsStack.empty()) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, frameBuffer1->GetColorBufferId());

        emptyQuadShader->Bind();
        emptyQuadShader->SetUniformInt("quadTexture", 1);

        RenderQuad();

        emptyQuadShader->Unbind();

		return;
	}

	for (int i = 0; i < effectsStack.size(); i++) {
        FrameBuffer* inputFrameBuffer = (i % 2 == 0) ? frameBuffer1 : frameBuffer2;
        FrameBuffer* outputFrameBuffer = (i % 2 == 1) ? frameBuffer1 : frameBuffer2;

        outputFrameBuffer->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, inputFrameBuffer->GetColorBufferId());

        effectsStack[i]->BindEffectShader();
        effectsStack[i]->SetUniforms();

        RenderQuad();  

        effectsStack[i]->UnbindEffectShader();
        outputFrameBuffer->Unbind();
	}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    FrameBuffer* finalFrameBuffer = (effectsStack.size() % 2 == 0) ? frameBuffer1 : frameBuffer2;

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, finalFrameBuffer->GetColorBufferId());

    emptyQuadShader->Bind();
    emptyQuadShader->SetUniformInt("quadTexture", 1);

    RenderQuad();

    emptyQuadShader->Unbind();
}

void PostProcessor::BindFirstFrameBuffer() {
	frameBuffer1->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessor::UnbindFirstFrameBuffer() {
	frameBuffer1->Unbind();
}

void PostProcessor::Initialize() {
    frameBuffer1 = new FrameBuffer();
    frameBuffer2 = new FrameBuffer();

    emptyQuadShader = new Shader(
        (std::string(Environment::GetRootPath()) + "/Shaders/EmptyQuadVert.glsl").c_str(),
        (std::string(Environment::GetRootPath()) + "/Shaders/EmptyQuadFrag.glsl").c_str());

    std::cout << (std::string(Environment::GetRootPath()) + "/Shaders/EmptyQuadVert.glsl").c_str() << std::endl;

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

void PostProcessor::AddEffect(PostProcessingEffect* effect) {
    effectsStack.push_back(effect);
}

void PostProcessor::RenderQuad() const {
    glBindVertexArray(quadVAOId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}