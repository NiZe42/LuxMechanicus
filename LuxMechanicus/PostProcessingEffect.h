#pragma once
#include "Shader.h"
#include "Environment.h"
#include <iostream>
#include <unordered_map>
#include <list>
#include "FrameBufferType.h"

class PostProcessingEffect
{
public:
	PostProcessingEffect();
	~PostProcessingEffect();

	void BindEffectShader() const;
	void UnbindEffectShader() const;

	virtual void SetUniforms() const;

	virtual void Initialize();

	FrameBufferType GetRequiredFrameBufferType() const;

protected:
	Shader* effectShader;

	FrameBufferType requiredFrameBufferType = FrameBufferType::None;
};

