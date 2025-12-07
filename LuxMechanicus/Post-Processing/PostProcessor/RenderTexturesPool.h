#pragma once
#include "..\..\Renderer\FrameBuffer\FrameBuffer.h"
#include <iostream>
#include "unordered_map"
#include "RenderTextureType.h"

class RenderTexturesPool
{
public:
	RenderTexturesPool();
	~RenderTexturesPool();

	void SaveRenderTextureById(RenderTextureType renderTextureType, unsigned int renderTextureId);

	unsigned int GetRenderTextureIdOfType(RenderTextureType renderTextureType);

private:

	std::unordered_map<RenderTextureType, unsigned int> pool;
};

