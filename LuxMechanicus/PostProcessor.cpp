#include "PostProcessor.h"

PostProcessor::PostProcessor() {

}

PostProcessor::~PostProcessor() {
    if (emptyQuadShader)
        delete(emptyQuadShader);
}

void PostProcessor::ApplyEffectsAndRender() {
	if (effectsStack.empty()) {
		
        emptyQuadShader->Bind();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, frameBuffer1->GetColorBufferId());

		return;
	}

		for (int i = 0; i < effectsStack.size(); i++) {
		
		}
}

void PostProcessor::BindFirstFrameBuffer() {
	frameBuffer1->Bind();
}

void PostProcessor::UnbindFirstFrameBuffer() {
	frameBuffer1->Unbind();
}

void PostProcessor::Initialize() {
    InitializeQuad();

    emptyQuadShader = new Shader(
        (std::string(Environment::GetRootPath()) + "/Shaders/EmptyQuadVert.glsl").c_str(),
        (std::string(Environment::GetRootPath()) + "/Shaders/EmptyQuadFrag.glsl").c_str());
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

void PostProcessor::AddEffect() {

}

void PostProcessor::RenderQuad() const {
    glBindVertexArray(quadVAOId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}