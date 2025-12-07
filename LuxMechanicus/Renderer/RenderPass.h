#pragma once
#include "..\Renderer\Shader\Shader.h"
#include "..\Utils\Environment.h"
#include <iostream>
#include <list>
#include "..\Post-Processing\PostProcessor\RenderTextureType.h"
#include "..\Renderer\FrameBuffer\FrameBuffer.h"
#include "..\Post-Processing\PostProcessor\RenderTexturesPool.h"

class RenderPass
{
public:
	RenderPass();
	~RenderPass();

	void BindEffectShader() const;
	void UnbindEffectShader() const;

	void BindFramebuffer() const;
	void UnbindFramebuffer() const;

	virtual void SetUniforms(RenderTexturesPool* renderTexturesPool) const;

	virtual void Apply(const unsigned int &quadVAOId, RenderTexturesPool* renderTexturesPool);

protected:
	Shader* effectShader = nullptr;
	FrameBuffer* frameBuffer = nullptr;

	std::vector<RenderTextureType> inputRenderTextures;

	// Max attached color buffers to a framebuffer is dependant on OpenGL version.
	// In our case it is 8.
	// TODO: Enforce maximum number of attachements based on OpenGL version.
	std::vector<RenderTextureType> outputRenderTextures;

	void RenderQuad(const unsigned int &quadVAOId) const;
}; 

