#pragma once
#include "GLIncludes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "RenderTextureType.h"
#include <vector>
#include <unordered_map>

class FrameBuffer
{
public:

	enum class DepthAttachmentType {
		NONE,
		RENDERBUFFER,
		TEXTURE_2D,
		TEXTURE_2D_ARRAY_LAYER
	};

	class Builder{
	public:
		
		Builder CreateFrameBuffer();
		Builder WithRenderTextures(
			const std::vector<RenderTextureType>& outputRenderTextures,
			int width,
			int height);
		Builder WithDepthRBO(int width, int height);
		Builder WithDepthTexture(int width, int height);
		Builder WithDepthTextureArray(int width, int height);
		FrameBuffer Build();
	private:
		FrameBuffer* frameBuffer;

	};

	FrameBuffer(
		const std::vector<RenderTextureType> &outputRenderTextures, 
		int screenWidth = 1920, 
		int screenHeight = 1080);
	~FrameBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetAttachedRenderTextureIdByType(RenderTextureType renderTextureType) const;
	unsigned int GetRBODepthId() const;

	unsigned int GetFrameBufferId() const;

private:

	FrameBuffer();
	std::unordered_map<RenderTextureType, unsigned int> attachedRenderTextures;

	unsigned int FBOId = 0, RBODepthId = 0;

	void Initialize(
		const std::vector<RenderTextureType> &outputRenderTextures, 
		int screenWidth,
		int screenHeight);
};

