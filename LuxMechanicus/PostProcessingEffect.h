#pragma once
#include "Shader.h"

class PostProcessingEffect
{
public:
	PostProcessingEffect();
	~PostProcessingEffect();

	void ActivateEffect() const;
	void DeactivateEffect() const;

protected:

	Shader mEffectShader;
};

