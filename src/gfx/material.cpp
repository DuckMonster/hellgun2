#include "material.h"
#include <stdio.h>
#include <stdlib.h>
#include "core/io.h"

Shader Shader::load_file(GLenum type, const char* path)
{
	Shader shader;

	// Read source file
	char* src;
	GLint src_len;
	load_whole_file(path, &src, &src_len);

	// Create and upload source
	shader.path = path;
	shader.type = type;
	shader.handle = glCreateShader(type);

	glShaderSource(shader.handle, 1, &src, &src_len);
	glCompileShader(shader.handle);

	// Compile result
	int result;
	glGetShaderiv(shader.handle, GL_COMPILE_STATUS, &result);
	shader.compile_result = result;

	if (!shader.compile_result)
	{
		static char BUFFER[1024];
		glGetShaderInfoLog(shader.handle, 1024, NULL, BUFFER);

		printf("Shader '%s' compile error:\n%s\n", path, BUFFER);
	}

	::free(src);
	return shader;
}

void Shader::free()
{
	glDeleteShader(handle);
	*this = Shader();
}

void Material::load_file(const char* vertex_path, const char* fragment_path)
{
	Array<Shader> shaders;
	shaders.add(Shader::load_file(GL_VERTEX_SHADER, vertex_path));
	shaders.add(Shader::load_file(GL_FRAGMENT_SHADER, fragment_path));

	link_program(shaders);

	// After link, we can free all the shaders
	for(auto& shader : shaders)
		shader.free();
}

void Material::load_file(const char* vertex_path, const char* geometry_path, const char* fragment_path)
{
	Array<Shader> shaders;
	shaders.add(Shader::load_file(GL_VERTEX_SHADER, vertex_path));
	shaders.add(Shader::load_file(GL_GEOMETRY_SHADER, geometry_path));
	shaders.add(Shader::load_file(GL_FRAGMENT_SHADER, fragment_path));

	link_program(shaders);

	// After link, we can free all the shaders
	for(auto& shader : shaders)
		shader.free();
}

void Material::link_program(const Array<Shader>& shaders)
{
	program = glCreateProgram();

	// First, check that all the shaders actually compiled correctly
	for(const auto& shader : shaders)
	{
		if (!shader.compile_result)
			return;
	}

	// Attach shaders
	for(const auto& shader : shaders)
		glAttachShader(program, shader.handle);

	glLinkProgram(program);

	// Link result
	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (!result)
	{
		static char BUFFER[1024];
		glGetProgramInfoLog(program, 1024, NULL, BUFFER);

		printf("Program link error:\n%s\n", BUFFER);
	}

	// Detach shaders
	for(const auto& shader : shaders)
		glDetachShader(program, shader.handle);
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