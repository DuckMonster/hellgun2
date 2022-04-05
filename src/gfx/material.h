#pragma once
#include "opengl.h"
#include "math/matrix.h"

struct Material
{
	GLuint program;

	void load_file(const char* vertex_path, const char* fragment_path);
	void load_source(const char* vertex_src, const char* fragment_src);
	void create_program(GLuint vertex, const char* vertex_name, GLuint fragment, const char* fragment_name);

	void use();

	void set(const char* name, const Mat4& mat);
	void set(const char* name, const Color& clr);
};