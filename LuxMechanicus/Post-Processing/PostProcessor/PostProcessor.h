#pragma once
#include "..\..\Renderer\FrameBuffer\FrameBuffer.h"
#include <iostream>
#include <vector>
#include "..\..\Renderer\RenderPass.h"
#include "..\..\Utils\Environment.h"
#include "..\PreliminaryPass\HDRRenderPass.h"
#include "RenderTexturesPool.h"
#include "..\Bloom\BloomEffect.h"
#include "..\..\Profiler\Profiler.h"

class PostProcessor
{
public:

	PostProcessor();
	~PostProcessor();

	void ApplyEffectsAndRender();

	void BindFirstFrameBuffer();
	void UnbindFirstFrameBuffer();

	FrameBuffer* GetFirstFrameBuffer();

private:
	unsigned int quadVAOId, quadVBOId;

	Shader* emptyQuadShader;
	FrameBuffer* firstFrameBuffer;
	RenderTexturesPool* renderTexturesPool;

	std::vector<RenderPass*> renderPassStack;

	void AddEffect(RenderPass* effect);

	void Initialize();
	void InitializeQuad();

	void RenderQuad() const;

};

