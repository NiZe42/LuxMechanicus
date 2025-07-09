#pragma once
#include "RenderPass.h"
#include "FrameBuffer.h"

class BloomEffect : public RenderPass {

public:
	BloomEffect();
	~BloomEffect();

	void Apply(const unsigned int& quadVAOId, RenderTexturesPool* renderTexturesPool) override;

private:

	FrameBuffer* pingPongFramebuffer1;
	FrameBuffer* pingPongFramebuffer2;

	Shader* blurShader;
};

