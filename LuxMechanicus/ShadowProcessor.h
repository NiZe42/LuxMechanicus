#pragma once
#include "GLIncludes.h"
#include <glm/glm.hpp>
#include <vector>
#include "LightData.h"

class ShadowProcessor
{
public:

	const static int SHADOW_MAP_RES = 1024;
	const static int ATLAS_RES = 4096;
	const static int SHADOWS_PER_ATLAS = (ATLAS_RES / SHADOW_MAP_RES) * (ATLAS_RES / SHADOW_MAP_RES); // 16 for 1024x1024 in 4096x4096

	ShadowProcessor();
	~ShadowProcessor();

	struct ShadowAtlas {
		GLuint textureID;
	};

private:
	void AllocateShadowAtlases(std::vector<LightData> lightdata);

	std::vector<ShadowAtlas> shadowAtlases;

};

