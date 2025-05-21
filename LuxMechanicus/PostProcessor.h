#pragma once
#include "Framebuffer.h"
#include <iostream>
#include <vector>
#include "PostProcessingEffect.h"
#include "Environment.h"
#include "HDREffect.h"

class PostProcessor
{
public:

	PostProcessor();
	~PostProcessor();

	void ApplyEffectsAndRender();

	virtual void BindFirstFrameBuffer();
	virtual void UnbindFirstFrameBuffer();

private:
	unsigned int quadVAOId, quadVBOId;

	Shader* emptyQuadShader;

	FrameBuffer* frameBuffer1;
	FrameBuffer* frameBuffer2;

	std::vector<PostProcessingEffect*> effectsStack;

	void AddEffect(PostProcessingEffect* effect);

	void Initialize();
	void InitializeQuad();

	void RenderQuad() const;

};

