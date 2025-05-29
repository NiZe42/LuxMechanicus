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

	FrameBuffer(const std::vector<RenderTextureType> &outputRenderTextures);
	~FrameBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetAttachedRenderTextureIdByType(RenderTextureType renderTextureType) const;

private:

	std::unordered_map<RenderTextureType, unsigned int> attachedRenderTextures;

	unsigned int FBOId = 0, RBODepthId = 0;

	void Initialize(const std::vector<RenderTextureType> &outputRenderTextures);
};

