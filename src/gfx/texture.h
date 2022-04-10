#pragma once
#include "opengl.h"

struct Texture
{
	GLuint handle;

	u32 width;
	u32 height;

	void init();
	void bind();
	void load_data(void* data, u32 width, u32 height, u8 channels);
};