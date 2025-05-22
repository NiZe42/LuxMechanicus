#pragma once
#include "Framebuffer.h"
#include <iostream>
#include <vector>
#include "PostProcessingEffect.h"
#include "Environment.h"
#include "HDREffect.h"
#include "FrameBufferPool.h"

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

	FrameBufferPool* frameBufferPool;

	std::vector<PostProcessingEffect*> effectsStack;

	void AddEffect(PostProcessingEffect* effect);

	void Initialize();
	void InitializeQuad();

	void RenderQuad() const;

};

