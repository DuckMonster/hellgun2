#pragma once
#include "opengl.h"

struct Shader
{
public:
	GLenum type;
	GLuint handle = GL_INVALID_INDEX;

	bool compile_result = false;
	String error;

	void compile(const String& source, GLenum shader_type);
	void free();
};