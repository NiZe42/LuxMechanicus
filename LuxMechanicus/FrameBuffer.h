#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>
#include "FrameBufferType.h"

class FrameBuffer
{
public:

	FrameBuffer(FrameBufferType pFrameBufferType);
	~FrameBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetColorBufferId() const;
	unsigned int GetBrightBufferId() const;

	FrameBufferType GetFrameBufferType() const;

private:
	const FrameBufferType frameBufferType = FrameBufferType::None;

	unsigned int FBOId = 0, 
		colorBufferId = 0, 
		brightBufferId = 0, 
		RBODepthId = 0;

	void Initialize(FrameBufferType pFrameBufferType);
};

