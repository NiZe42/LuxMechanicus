#include "BloomEffect.h"

BloomEffect::BloomEffect() {
	blurShader = new Shader(
		(std::string(Environment::GetRootPath()) + "/Shaders/BlurVert.glsl").c_str(),
		(std::string(Environment::GetRootPath()) + "/Shaders/BlurFrag.glsl").c_str());

	effectShader = new Shader(
		(std::string(Environment::GetRootPath()) + "/Shaders/BloomCombineVert.glsl").c_str(),
		(std::string(Environment::GetRootPath()) + "/Shaders/BloomCombineFrag.glsl").c_str());

	inputRenderTextures.emplace_back(RenderTextureType::COLOR);
	inputRenderTextures.emplace_back(RenderTextureType::BRIGHT);
	outputRenderTextures.emplace_back(RenderTextureType::COLOR);

	pingPongFramebuffer1 = FrameBuffer::Builder::Builder()
		.WithRenderTextures({ RenderTextureType::BRIGHT })
		.Build();

	pingPongFramebuffer2 = FrameBuffer::Builder::Builder()
		.WithRenderTextures({ RenderTextureType::BRIGHT })
		.Build();

	frameBuffer = FrameBuffer::Builder::Builder()
		.WithRenderTextures(outputRenderTextures)
		.Build();
}

BloomEffect::~BloomEffect() {
	if (effectShader)
		delete effectShader;

	if (frameBuffer)
		delete frameBuffer;

	if (pingPongFramebuffer1)
		delete pingPongFramebuffer1;
	
	if (pingPongFramebuffer2)
		delete pingPongFramebuffer2;

	if (blurShader)
		delete blurShader;
}

void BloomEffect::Apply(const unsigned int& quadVAOId, RenderTexturesPool* renderTexturesPool) {

	int blurIterationNum = 10;
	bool isHorizontal = true;

	blurShader->Bind();

	for (int i = 0; i < blurIterationNum; i++) {
		
		FrameBuffer* currentFramebuffer = i % 2 == 0 ? pingPongFramebuffer1 : pingPongFramebuffer2;
		FrameBuffer* inputFrameBuffer = i % 2 == 1 ? pingPongFramebuffer1 : pingPongFramebuffer2;

		currentFramebuffer->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);

		if (i == 0) {
			glBindTexture(GL_TEXTURE_2D, renderTexturesPool->GetRenderTextureIdOfType(RenderTextureType::BRIGHT));
			blurShader->SetUniformInt("image", 0);
		} else {
			glBindTexture(GL_TEXTURE_2D, inputFrameBuffer->GetAttachedRenderTextureIdByType(RenderTextureType::BRIGHT));
			blurShader->SetUniformInt("image", 0);
		}
		blurShader->SetUniformBool("isHorizontal", isHorizontal);
		
		RenderQuad(quadVAOId);

		currentFramebuffer->Unbind();

		isHorizontal = !isHorizontal;
	}
	blurShader->Unbind();

	FrameBuffer* inputFrameBuffer = blurIterationNum % 2 == 1 ? pingPongFramebuffer1 : pingPongFramebuffer2;

	frameBuffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	effectShader->Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inputFrameBuffer->GetAttachedRenderTextureIdByType(RenderTextureType::BRIGHT));
	effectShader->SetUniformInt("blurredBrightMap", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, renderTexturesPool->GetRenderTextureIdOfType(RenderTextureType::COLOR));
	effectShader->SetUniformInt("colorMap", 1);

	RenderQuad(quadVAOId);

	effectShader->Unbind();
	frameBuffer->Unbind();

	for (int i = 0; i < outputRenderTextures.size(); i++) {
		RenderTextureType renderTextureType = outputRenderTextures[i];
		unsigned int renderTextureId = frameBuffer->GetAttachedRenderTextureIdByType(renderTextureType);
		renderTexturesPool->SaveRenderTextureById(renderTextureType, renderTextureId);
	}
}
