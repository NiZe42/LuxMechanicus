#pragma once
#include "RenderPass.h"
#include "FrameBuffer.h"

class BloomEffect : public RenderPass {

public:
	BloomEffect();
	~BloomEffect();

	void SetUniforms(RenderTexturesPool* renderTexturesPool) const override;

private:

};

