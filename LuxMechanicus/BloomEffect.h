#pragma once
#include "PostProcessingEffect.h"
#include "FrameBuffer.h"

class BloomEffect : public PostProcessingEffect {

public:
	BloomEffect();
	~BloomEffect();

	void SetUniforms() const override;

private:

};

