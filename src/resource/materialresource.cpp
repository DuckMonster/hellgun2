#include "materialresource.h"
#include "shaderresource.h"

void Material_Resource::load()
{
	clear_dependencies();

	// Load DAT file describing the material
	dat.load_file(get_absolute_path());

	if (!dat.contains_value("vertex"))
	{
		printf("[MATERIAL ERROR] 'vertex' not specified\n");
		return;
	}
	if (!dat.contains_value("fragment"))
	{
		printf("[MATERIAL ERROR] 'fragment' not specified\n");
		return;
	}

	String vert_src = dat.read_str("vertex");
	String frag_src = dat.read_str("fragment");

	Array<Shader*> shaders;
	Shader_Resource* vertex = Resource::find_or_load_resource<Shader_Resource>(vert_src);
	Shader_Resource* fragment = Resource::find_or_load_resource<Shader_Resource>(frag_src);
	add_dependency(vertex);
	add_dependency(fragment);

	shaders.add(&vertex->shader);
	shaders.add(&fragment->shader);

	// Optional geometry shader
	if (dat.contains_value("geometry"))
	{
		String geom_src = dat.read_str("geometry");
		Shader_Resource* geometry = Resource::find_or_load_resource<Shader_Resource>(geom_src);

		add_dependency(geometry);
		shaders.add(&geometry->shader);
	}

	// Don't try to link if any of the shaders had compile failures
	for(Shader* shader : shaders)
	{
		if (!shader->compile_result)
			return;
	}

	material.link_program(shaders);
	if (!material.link_result)
		printf("[MATERIAL ERROR] '%s' failed to link --\n%s\n-------------\n", path.data(), material.error.data());
}

void Material_Resource::free()
{

}