#pragma once
#include "FrameBuffer.h"
#include <iostream>
#include "unordered_map"
#include "RenderTextureType.h"

class RenderTexturesPool
{
public:
	RenderTexturesPool();
	~RenderTexturesPool();

	void SaveRenderTextureId(RenderTextureType renderTextureType, unsigned int renderTextureId);

	unsigned int GetRenderTextureIdOfType(RenderTextureType renderTextureType);

private:

	std::unordered_map<RenderTextureType, unsigned int> pool;
};

