#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\..\stb_image.h"

Texture::Texture(const char* texturePath) {
	
	glGenTextures(1, &mTextureId);
	
	glBindTexture(GL_TEXTURE_2D, mTextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	LoadTextureFromPath(texturePath);

	engineTextureId = std::hash<std::string>()(texturePath);
}

Texture::~Texture() {
	
}

void Texture::Bind() const {
	glBindTexture(GL_TEXTURE_2D, mTextureId);
}

unsigned int Texture::GetTextureId() const {
	return mTextureId;
}

void Texture::LoadTextureFromPath(const char* texturePath) {
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

