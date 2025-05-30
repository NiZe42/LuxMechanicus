#pragma once
#include "RenderPass.h"

class HDRRenderPass : public RenderPass {

public:
	HDRRenderPass();
	~HDRRenderPass();

	void SetUniforms(RenderTexturesPool* renderTexturesPool) const override;

	void Apply(const unsigned int& quadVAOId, RenderTexturesPool* renderTexturesPool) override;
private:
	
};

