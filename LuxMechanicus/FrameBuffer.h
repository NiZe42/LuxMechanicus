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

	FrameBuffer();
	~FrameBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetColorBufferId() const;
	unsigned int GetBrightBufferId() const;

	FrameBufferType GetFrameBufferType() const;

private:
	const FrameBufferType frameBufferType;

	unsigned int FBOId, colorBufferId, brightBufferId, RBODepthId;

	void Initialize();
};

