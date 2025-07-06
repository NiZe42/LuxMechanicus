#include "HDRRenderPass.h"

HDRRenderPass::HDRRenderPass() {
	effectShader = new Shader(
		(std::string(Environment::GetRootPath()) + "/Shaders/HDRShaderVert.glsl").c_str(),
		(std::string(Environment::GetRootPath()) + "/Shaders/HDRShaderFrag.glsl").c_str());

	inputRenderTextures.emplace_back(RenderTextureType::COLOR);
	outputRenderTextures.emplace_back(RenderTextureType::COLOR);
	outputRenderTextures.emplace_back(RenderTextureType::BRIGHT);

	frameBuffer = FrameBuffer::Builder::Builder()
		.WithRenderTextures(outputRenderTextures)
		.Build();
}

HDRRenderPass::~HDRRenderPass() {
	if (effectShader)
		delete effectShader;

	if (frameBuffer)
		delete frameBuffer;
}

void HDRRenderPass::SetUniforms(RenderTexturesPool* renderTexturesPool) const {

	for (int i = 0; i < inputRenderTextures.size(); i++) {
		RenderTextureType renderTextureType = inputRenderTextures[i];
		std::string renderTextureTypeName = RenderTextureTypeHelper::ToString(renderTextureType);
		
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, renderTexturesPool->GetRenderTextureIdOfType(renderTextureType));
		
		effectShader->SetUniformInt(renderTextureTypeName, i);
	}

	//effectShader->SetUniformBool("useHDR", true);
	//effectShader->SetUniformFloat("exposure", 0.5f);
}

void HDRRenderPass::Apply(const unsigned int& quadVAOId, RenderTexturesPool* renderTexturesPool) {
	BindFramebuffer();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BindEffectShader();
	SetUniforms(renderTexturesPool);

	RenderQuad(quadVAOId);

	UnbindEffectShader();

	UnbindFramebuffer();

	for (int i = 0; i < outputRenderTextures.size(); i++) {
		
		RenderTextureType renderTextureType = outputRenderTextures[i];
		unsigned int renderTextureId = frameBuffer->GetAttachedRenderTextureIdByType(renderTextureType);

		renderTexturesPool->SaveRenderTextureById(renderTextureType, renderTextureId);
	}
}
