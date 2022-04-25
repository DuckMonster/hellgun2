#include "shader.h"

void Shader::compile(const String& source, GLenum shader_type)
{
	free();

	// Attach source and compile
	type = shader_type;
	handle = glCreateShader(type);

	const char* data = source.data();
	glShaderSource(handle, 1, &data, nullptr);
	glCompileShader(handle);

	// Compile result
	int _result;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &_result);

	compile_result = _result;
	if (!compile_result)
	{
		// Fetch length first
		GLsizei error_length;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &error_length);

		// Then fetch error
		error.resize(error_length);
		glGetShaderInfoLog(handle, error_length, nullptr, error.data());
	}
}

void Shader::free()
{
	if (handle == GL_INVALID_INDEX)
		return;

	glDeleteShader(handle);
	handle = GL_INVALID_INDEX;
	compile_result = false;
}