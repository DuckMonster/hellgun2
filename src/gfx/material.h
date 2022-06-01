#pragma once
#include "opengl.h"
#include "math/matrix.h"
#include "container/array.h"
#include "shader.h"

struct Material
{
	GLuint program = GL_INVALID_INDEX;
	bool link_result = false;
	String error;

	void link_program(const Array<Shader*>& shaders);
	void free();

	void use();

	void set(const char* name, const float& value);
	void set(const char* name, const Mat4& mat);
	void set(const char* name, const Color& clr);
	void set(const char* name, const Vec2& vec);
};