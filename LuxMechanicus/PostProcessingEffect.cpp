#include "PostProcessingEffect.h"

PostProcessingEffect::PostProcessingEffect() {

}

PostProcessingEffect::~PostProcessingEffect() {
    if (effectShader)
        delete effectShader;
}

void PostProcessingEffect::BindEffectShader() const {
    if (effectShader)
        effectShader->Bind();
}

void PostProcessingEffect::UnbindEffectShader() const {
    if (effectShader)
        effectShader->Unbind();
}

void PostProcessingEffect::SetUniforms() const {
    
}

// Important to call this after instantiation so we would minimize Shader Creation
void PostProcessingEffect::Initialize() {
    if (!effectShader) {
    effectShader = new Shader(
        (std::string(Environment::GetRootPath()) + "/Shaders/EmptyQuadVert.glsl").c_str(),
        (std::string(Environment::GetRootPath()) + "/Shaders/EmptyQuadFrag.glsl").c_str());
    }

    //It is really important that every PP shader would have Sampler2D variable named "quadTexture".
    effectShader->Bind();
    effectShader->SetUniformInt("quadTexture", 1);
    effectShader->Unbind();
}   

FrameBufferType PostProcessingEffect::GetRequiredFrameBufferType() const {
    return requiredFrameBufferType;
}
