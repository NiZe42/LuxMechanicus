#pragma once
#include "PostProcessingEffect.h"

class HDREffect : public PostProcessingEffect {

public:
	HDREffect();
	~HDREffect();

	void SetUniforms() const override;

	void Initialize() override;

private:
		
};

