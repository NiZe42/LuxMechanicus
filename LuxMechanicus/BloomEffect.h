#pragma once
#include "RenderPass.h"
#include "FrameBuffer.h"

class BloomEffect : public RenderPass {

public:
	BloomEffect();
	~BloomEffect();

	void SetUniforms(RenderTexturesPool* renderTexturesPool) const override;

	void Apply(const unsigned int& quadVAOId, RenderTexturesPool* renderTexturesPool) override;
private:

	FrameBuffer pinpPongFramebuffer1;
	FrameBuffer pingPongFramebuffer2;

};

