#pragma once
#include "RenderPass.h"

class HDRRenderPass : public RenderPass {

public:
	HDRRenderPass();
	~HDRRenderPass();

	void SetUniforms(RenderTexturesPool* renderTexturesPool) const override;

private:
	
};

