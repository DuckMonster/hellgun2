#define GL_IMPLEMENT
#include "opengl.h"
#include <stdio.h>

template<typename T>
void _load_gl(T* func, const char* name)
{
	*func = (T)wglGetProcAddress(name);
	if (*func == nullptr)
		printf("Failed to load OpenGL extension '%s'\n", name);
}
#define LOAD_GL(func) _load_gl(&func, #func)

void handle_gl_debug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param)
{
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;

	//printf("[OPENGL] '%s'\n", message);
}

void load_gl_extensions()
{
	// VBO
	LOAD_GL(glGenBuffers);
	LOAD_GL(glDeleteBuffers);
	LOAD_GL(glBindBuffer);
	LOAD_GL(glBufferData);
	LOAD_GL(glBufferSubData);

	// VAO
	LOAD_GL(glGenVertexArrays);
	LOAD_GL(glDeleteVertexArrays);
	LOAD_GL(glBindVertexArray);
	LOAD_GL(glEnableVertexAttribArray);
	LOAD_GL(glVertexAttribPointer);

	// SHADERS
	LOAD_GL(glCreateShader);
	LOAD_GL(glDeleteShader);
	LOAD_GL(glShaderSource);
	LOAD_GL(glCompileShader);
	LOAD_GL(glGetShaderiv);
	LOAD_GL(glGetShaderInfoLog);

	LOAD_GL(glCreateProgram);
	LOAD_GL(glDeleteProgram);
	LOAD_GL(glAttachShader);
	LOAD_GL(glDetachShader);
	LOAD_GL(glLinkProgram);
	LOAD_GL(glUseProgram);
	LOAD_GL(glGetProgramiv);
	LOAD_GL(glGetProgramInfoLog);

	// UNIFORMS
	LOAD_GL(glGetUniformLocation);

	LOAD_GL(glUniform1f);
	LOAD_GL(glUniform2fv);
	LOAD_GL(glUniform4fv);
	LOAD_GL(glUniformMatrix4fv);

	// Setup debug stuff
	GL_PROC(PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback);
	LOAD_GL(glDebugMessageCallback);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(handle_gl_debug, NULL);
}