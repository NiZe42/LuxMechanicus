#include "HDREffect.h"

HDREffect::HDREffect() {
	effectShader = new Shader(
		(std::string(Environment::GetRootPath()) + "/Shaders/HDRShaderVert.glsl").c_str(),
		(std::string(Environment::GetRootPath()) + "/Shaders/HDRShaderFrag.glsl").c_str());

	requiredFrameBufferType = REQUIREDFRAMEBUFFERTYPE;
}

HDREffect::~HDREffect() {

}

void HDREffect::SetUniforms() const {
	effectShader->SetUniformBool("useHDR", true);
	effectShader->SetUniformFloat("exposure", 0.5f);
}

void HDREffect::Initialize() {

}