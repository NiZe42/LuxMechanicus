#include "BloomEffect.h"

BloomEffect::BloomEffect() {
	effectShader = new Shader(
		(std::string(Environment::GetRootPath()) + "/Shaders/HDRShaderVert.glsl").c_str(),
		(std::string(Environment::GetRootPath()) + "/Shaders/HDRShaderFrag.glsl").c_str());

	inputRenderTextures.emplace_back(RenderTextureType::ColorMap);
	inputRenderTextures.emplace_back(RenderTextureType::BrightMap);
	outputRenderTextures.emplace_back(RenderTextureType::ColorMap);

	pingPongFramebuffer1 = new FrameBuffer(outputRenderTextures);
}

BloomEffect::~BloomEffect() {
	if (effectShader)
		delete effectShader;

	if (frameBuffer)
		delete frameBuffer;
}

void BloomEffect::SetUniforms(RenderTexturesPool* renderTexturesPool) const {

	for (int i = 0; i < inputRenderTextures.size(); i++) {
		RenderTextureType renderTextureType = inputRenderTextures[i];
		std::string renderTextureTypeName = RenderTextureTypeHelper::ToString(renderTextureType);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, renderTexturesPool->GetRenderTextureIdOfType(renderTextureType));

		effectShader->SetUniformInt(renderTextureTypeName, i);
	}

	effectShader->SetUniformBool("useHDR", true);
	effectShader->SetUniformFloat("exposure", 0.5f);
}

void BloomEffect::Apply(const unsigned int& quadVAOId, RenderTexturesPool* renderTexturesPool) {
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

		renderTexturesPool->SaveRenderTextureId(renderTextureType, renderTextureId);
	}
}
