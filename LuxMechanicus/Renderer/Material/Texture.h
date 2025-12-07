#pragma once
#include "..\..\Utils\GLIncludes.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Texture
{
public:
	
	Texture(const char* texturePath);
	~Texture();

	void Bind() const;

	unsigned int GetTextureId() const;

protected:
	unsigned int mTextureId;

	void LoadTextureFromPath(const char* texturePath);
};

