#pragma once
#include "Framebuffer.h"
#include <iostream>
#include <vector>
#include "PostProcessingEffect.h"
#include "Environment.h"

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

	FrameBuffer* frameBuffer1;
	FrameBuffer* frameBuffer2;

	std::vector<std::unique_ptr<PostProcessingEffect>> effectsStack;

	void AddEffect();

	void Initialize();
	void InitializeQuad();

	void RenderQuad() const;

};

