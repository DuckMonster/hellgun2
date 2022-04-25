#include "material.h"
#include <stdio.h>
#include <stdlib.h>
#include "core/io.h"

void Material::link_program(const Array<Shader*>& shaders)
{
	free();
	program = glCreateProgram();
	link_result = false;

	// First, check that all the shaders actually compiled correctly
	for(const auto* shader : shaders)
	{
		if (!shader->compile_result)
		{
			error = "Shaders had compile errors";
			return;
		}
	}

	// Attach shaders
	for(const auto* shader : shaders)
		glAttachShader(program, shader->handle);

	glLinkProgram(program);

	// Link result
	GLint _result;
	glGetProgramiv(program, GL_LINK_STATUS, &_result);

	link_result = _result;

	if (!link_result)
	{
		// Fetch length first
		GLsizei error_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &error_length);

		// Then fetch error
		error.resize(error_length);
		glGetProgramInfoLog(program, error_length, nullptr, error.data());
	}

	// Detach shaders
	for(const auto* shader : shaders)
		glDetachShader(program, shader->handle);
}

void Material::free()
{
	if (program == GL_INVALID_INDEX)
		return;

	glDeleteProgram(program);
	program = GL_INVALID_INDEX;
}

void Material::use()
{
	glUseProgram(program);
}

void Material::set(const char* name, const float& value)
{
	GLint uniform = glGetUniformLocation(program, name);
	if (uniform == -1)
	{
		//printf("Uniform '%s' not found\n", name);
		return;
	}

	glUniform1f(uniform, value);
}

void Material::set(const char* name, const Mat4& mat)
{
	GLint uniform = glGetUniformLocation(program, name);
	if (uniform == -1)
	{
		//printf("Uniform '%s' not found\n", name);
		return;
	}

	glUniformMatrix4fv(uniform, 1, false, (float*)&mat);
}

void Material::set(const char* name, const Color& clr)
{
	GLint uniform = glGetUniformLocation(program, name);
	if (uniform == -1)
	{
		//printf("Uniform '%s' not found\n", name);
		return;
	}

	glUniform4fv(uniform, 1, (float*)&clr);
}