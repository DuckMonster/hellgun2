#pragma once
#include "opengl.h"
#include "math/matrix.h"
#include "container/array.h"

struct Shader
{
	static Shader load_file(GLenum type, const char* path);

	const char* path = nullptr;
	GLenum type;
	GLuint handle;

	bool compile_result = false;

	void free();
};

struct Material
{
	GLuint program;

	void load_file(const char* vertex_path, const char* fragment_path);
	void load_file(const char* vertex_path, const char* geometry_path, const char* fragment_path);

	void link_program(const Array<Shader>& shaders);

	void use();

	void set(const char* name, const float& value);
	void set(const char* name, const Mat4& mat);
	void set(const char* name, const Color& clr);
};