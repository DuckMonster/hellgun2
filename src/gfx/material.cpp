#include "material.h"
#include <stdio.h>
#include <stdlib.h>
#include "core/io.h"

static GLuint load_shader(const char* name, GLenum type, const char* src, GLint src_len = -1)
{
	GLuint shader = glCreateShader(type);

	GLint* len_ptr = NULL;
	if (src_len >= 0)
		len_ptr = &src_len;

	glShaderSource(shader, 1, &src, len_ptr);
	glCompileShader(shader);

	// Compile result
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		static char BUFFER[1024];
		glGetShaderInfoLog(shader, 1024, NULL, BUFFER);

		printf("Shader '%s' compile error:\n%s\n", name, BUFFER);
	}

	return shader;
}

void Material::load_file(const char* vertex_path, const char* fragment_path)
{
	// Load vertex shader
	char* vertex_src;
	u64 vertex_len;
	load_whole_file(vertex_path, &vertex_src, &vertex_len);

	GLuint vertex = load_shader(vertex_path, GL_VERTEX_SHADER, vertex_src, vertex_len);
	free(vertex_src);

	// Load fragment shader
	char* fragment_src;
	u64 fragment_len;
	load_whole_file(fragment_path, &fragment_src, &fragment_len);

	GLuint fragment = load_shader(fragment_path, GL_FRAGMENT_SHADER, fragment_src, fragment_len);
	free(fragment_src);

	create_program(vertex, vertex_path, fragment, fragment_path);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Material::load_source(const char* vertex_src, const char* fragment_src)
{
	GLuint vertex = load_shader("inline_vert", GL_VERTEX_SHADER, vertex_src);
	GLuint fragment = load_shader("inline_frag", GL_FRAGMENT_SHADER, fragment_src);

	create_program(vertex, "inline_vert", fragment, "inline_frag");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Material::create_program(GLuint vertex, const char* vertex_name, GLuint fragment, const char* fragment_name)
{
	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	// Link result
	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (!result)
	{
		static char BUFFER[1024];
		glGetProgramInfoLog(program, 1024, NULL, BUFFER);

		printf("Program '%s'/'%s' link error:\n%s\n", vertex_name, fragment_name, BUFFER);
	}

	// Detach shaders
	glDetachShader(program, vertex);
	glDetachShader(program, fragment);
}

void Material::use()
{
	glUseProgram(program);
}

void Material::set(const char* name, const Mat4& mat)
{
	GLint uniform = glGetUniformLocation(program, name);
	if (uniform == -1)
	{
		printf("Uniform '%s' not found\n", name);
		return;
	}

	glUniformMatrix4fv(uniform, 1, false, (float*)&mat);
}

void Material::set(const char* name, const Color& clr)
{
	GLint uniform = glGetUniformLocation(program, name);
	if (uniform == -1)
	{
		printf("Uniform '%s' not found\n", name);
		return;
	}

	glUniform4fv(uniform, 1, (float*)&clr);
}