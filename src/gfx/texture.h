#pragma once
#include "opengl.h"

struct Texture
{
	GLuint handle;

	void init();
	void bind();
	void load_data(void* data, int width, int height, int channels);
};