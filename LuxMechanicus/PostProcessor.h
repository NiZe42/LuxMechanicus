#pragma once
#include "Framebuffer.h"
#include <iostream>
#include <vector>
#include "RenderPass.h"
#include "Environment.h"
#include "HDRRenderPass.h"
#include "RenderTexturesPool.h"

class PostProcessor
{
public:

	PostProcessor();
	~PostProcessor();

	void ApplyEffectsAndRender();

	void BindFirstFrameBuffer();
	void UnbindFirstFrameBuffer();

private:
	unsigned int quadVAOId, quadVBOId;

	Shader* emptyQuadShader;
	FrameBuffer* firstFrameBuffer;
	RenderTexturesPool* renderTexturesPool;

	std::vector<RenderPass*> renderPassStack;

	void AddEffect(RenderPass* effect);

	void Initialize();
	void InitializeFirstFramebuffer();
	void InitializeQuad();

	void RenderQuad() const;

};

