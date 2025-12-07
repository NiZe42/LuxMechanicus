#pragma once
#include "..\..\Utils\GLIncludes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "..\..\Post-Processing\PostProcessor\RenderTextureType.h"
#include <vector>
#include <unordered_map>

class FrameBuffer {
public:

	enum class DepthAttachmentType {
		NONE,
		RENDERBUFFER,
		TEXTURE_2D,
		TEXTURE_2D_LAYERED_ARRAY
	};


	class Builder{
	public:
		
		Builder();
		Builder& WithRenderTextures(
			const std::vector<RenderTextureType>& outputRenderTextures,
			int width = 1920,
			int height = 1080);
		Builder& WithDepthRBO(int width = 1920, int height = 1080);
		Builder& WithDepthTexture(int width = 1920, int height = 1080);
		Builder& WithDepthTextureArray(int count = 8, int width = 1920, int height = 1080);
		FrameBuffer* Build();
	private:
		FrameBuffer* frameBuffer;
	};

	~FrameBuffer();

	void Bind() const;
	void Unbind() const;

	DepthAttachmentType GetDepthAttachmentType() const;
	void SetDepthAttachmentType(DepthAttachmentType newDepthAttachmentType);

	unsigned int GetAttachedRenderTextureIdByType(RenderTextureType renderTextureType) const;
	
	unsigned int GetRBODepthId() const;
	unsigned int GetFrameBufferId() const;
	unsigned int GetDepthTextureId() const;
	unsigned int GetDepthArrayId() const;

private:

	FrameBuffer();

	std::unordered_map<RenderTextureType, unsigned int> attachedRenderTextures;

	unsigned int FBOId = 0, RBODepthId = 0;
	unsigned int depthTextureId = 0, depthArrayId = 0;

	DepthAttachmentType depthAttachmentType = DepthAttachmentType::NONE;

	void CheckFrameBuffer() const;
};

static class DepthAttachmentHelper {
public:
	static inline std::string ToString(FrameBuffer::DepthAttachmentType type) {
		switch (type) {
		case FrameBuffer::DepthAttachmentType::NONE: return "{None}";
		case FrameBuffer::DepthAttachmentType::RENDERBUFFER: return "{RenderBuffer}";
		case FrameBuffer::DepthAttachmentType::TEXTURE_2D: return "{Texture}";
		case FrameBuffer::DepthAttachmentType::TEXTURE_2D_LAYERED_ARRAY: return "{Layered Texture Array}";
		default: {
			std::cerr << "Incorrect Depth Attachment Type";
			return "";
		}
		}
	}
};

