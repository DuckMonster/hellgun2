#include "shaderresource.h"
#include "core/io.h"

void Shader_Resource::load()
{
	String source = load_whole_file(get_absolute_path().data());
	if (source.length() == 0)
		return;

	// Determine shader type from file name
	GLenum type = GL_INVALID_INDEX;

	if (path.ends_with(".frag"))
		type = GL_FRAGMENT_SHADER;
	if (path.ends_with(".vert"))
		type = GL_VERTEX_SHADER;
	if (path.ends_with(".geom"))
		type = GL_GEOMETRY_SHADER;

	// Couldn't find a type...
	if (type == GL_INVALID_INDEX)
	{
		printf("[SHADER ERROR] Couldn't determine shader type for file '%s'\n", path.data());
		return;
	}

	shader.compile(source, type);

	if (!shader.compile_result)
		printf("[SHADER ERROR] '%s' failed to compile --\n%s\n---------\n", path.data(), shader.error.data());
}

void Shader_Resource::free()
{
	shader.free();
}