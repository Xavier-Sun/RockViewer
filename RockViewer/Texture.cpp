#include "Texture.h"

#include <glad/glad.h>

Texture::Texture()
{
	ID = 0u;
}

void Texture::Create()
{
	glGenTextures(1, &ID);
}

void Texture::Upload(unsigned int width, unsigned int height, unsigned char* data)
{
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Destroy()
{
	glDeleteTextures(1, &ID);
}