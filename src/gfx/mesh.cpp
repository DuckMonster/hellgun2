#include "mesh.h"

void Mesh::init()
{
	glGenVertexArrays(1, &vao);
}

void Mesh::add_buffer(u32 idx)
{
	glGenBuffers(1, buffers + idx);
}
void Mesh::bind_attribute(u32 buffer_idx, u32 attr_idx, u32 num_elements, u64 stride, u64 offset)
{
	bind();
	glBindBuffer(GL_ARRAY_BUFFER, buffers[buffer_idx]);
	glEnableVertexAttribArray(attr_idx);
	glVertexAttribPointer(attr_idx, num_elements, GL_FLOAT, false, stride, (void*)offset);
}

void Mesh::buffer_data(u32 idx, u64 size, const void* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffers[idx]);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void Mesh::add_element_buffer()
{
	glGenBuffers(1, &elem_buffer);
}

void Mesh::element_data(u64 size, const void* data)
{
	bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void Mesh::bind()
{
	glBindVertexArray(vao);
}

void Mesh::draw()
{
	glBindVertexArray(vao);

	if (elem_buffer == GL_INVALID_INDEX)
	{
		glDrawArrays(draw_mode, draw_offset, draw_num);
	}
	else
	{
		glDrawElements(draw_mode, draw_num, GL_UNSIGNED_INT, (void*)draw_offset);
	}
}