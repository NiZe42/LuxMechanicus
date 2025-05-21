#pragma once
#include "FrameBuffer.h"
#include <iostream>
#include "unordered_map"
#include "FrameBufferType.h"

class FrameBufferPool
{
public:

	FrameBufferPool();
	~FrameBufferPool();

	void AddFrameBuffer(FrameBuffer* frameBuffer);

	FrameBuffer* GetFrameBufferOfType(FrameBufferType framebufferType) const;

private:
	std::unordered_map<FrameBufferType, FrameBuffer*> pool;

	void Initialize();
};

