#pragma once
#include "Shader.h"
#include "Environment.h"

class PostProcessingEffect
{
public:
	PostProcessingEffect();
	~PostProcessingEffect();

	void BindEffectShader() const;
	void UnbindEffectShader() const;

	virtual void SetUniforms() const;

	void Initialize();

protected:

	Shader* effectShader;
};

