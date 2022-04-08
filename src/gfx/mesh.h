#pragma once
#define MAX_BUFFERS 4
#include "gfx/opengl.h"

struct Mesh
{
	GLuint vao;
	GLuint buffers[MAX_BUFFERS];
	GLuint elem_buffer = GL_INVALID_INDEX;

	GLenum draw_mode = GL_TRIANGLES;
	u32 draw_offset = 0;
	u32 draw_num = 0;

	void init();
	void add_buffer(u32 idx);

	void bind_attribute(u32 buffer_idx, u32 attr_idx, u32 num_elements, u64 stride = 0, u64 offset = 0);
	void buffer_data(u32 idx, u64 size, const void* data);

	void add_element_buffer();
	void element_data(u64 size, const void* data);

	void bind();
	void draw();
};