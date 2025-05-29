#include "HDRRenderPass.h"

HDRRenderPass::HDRRenderPass() {
	effectShader = new Shader(
		(std::string(Environment::GetRootPath()) + "/Shaders/HDRShaderVert.glsl").c_str(),
		(std::string(Environment::GetRootPath()) + "/Shaders/HDRShaderFrag.glsl").c_str());

	inputRenderTextures.emplace_back(RenderTextureType::ColorMap);
	outputRenderTextures.emplace_back(RenderTextureType::ColorMap);
	outputRenderTextures.emplace_back(RenderTextureType::BrightMap);

	frameBuffer = new FrameBuffer(outputRenderTextures);
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

	effectShader->SetUniformBool("useHDR", true);
	effectShader->SetUniformFloat("exposure", 0.5f);
}
