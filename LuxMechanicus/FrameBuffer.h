#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>

class FrameBuffer
{
public:

	FrameBuffer();
	~FrameBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetColorBufferId() const;

private:
	unsigned int FBOId, colorBufferId, RBODepthId;

	void Initialize();
	
};

