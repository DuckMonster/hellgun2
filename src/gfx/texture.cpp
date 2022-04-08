#include "texture.h"

void Texture::init()
{
	glGenTextures(1, &handle);

	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::load_data(void* data, int width, int height, int channels)
{
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}