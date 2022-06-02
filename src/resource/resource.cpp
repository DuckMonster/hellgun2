#include "resource.h"
#include "gfx/material.h"
#include "gfx/texture.h"
#include "core/io.h"
#include "import/tga.h"
#include "datresource.h"
#include "meshresource.h"
#include "shaderresource.h"
#include "materialresource.h"
#include "textureresource.h"
#include "levelresource.h"
#include <stdio.h>

String Resource::resource_root = "res/";
Map<String, void*> Resource::old_resource_map;
Map<String, Resource*> Resource::resource_map;
Array<Resource*> Resource::resources;

void Resource::register_resource(const String& str, void* data)
{
	old_resource_map.add(str, data);
	printf("Resource '%s' loaded\n", str.data());
}

Mesh* Resource::load_mesh(const String& path)
{
	return &find_or_load_resource<Mesh_Resource>(path)->mesh;
}

Material* Resource::load_material(const String& path)
{
	return &find_or_load_resource<Material_Resource>(path)->material;
}

Shader* Resource::load_shader(const String& path)
{
	Shader_Resource* res = find_or_load_resource<Shader_Resource>(path);
	return &res->shader;
}

Texture* Resource::load_texture(const String& path)
{
	return &find_or_load_resource<Texture_Resource>(path)->texture;
}

Level* Resource::load_level(const String& path)
{
	return &find_or_load_resource<Level_Resource>(path)->level;
}

Dat_File* Resource::load_dat(const String& path)
{
	Dat_Resource* res = find_or_load_resource<Dat_Resource>(path);
	return &res->dat;
}

void Resource::update_hotreload()
{
	bool had_hotreload = false;
	Stop_Watch watch;

	Array<Resource*> resources_copy = resources;
	for(auto* resource : resources_copy)
	{
		u64 new_time = get_file_modify_time(resource->get_absolute_path().data());
		if (new_time != resource->file_time)
		{
			hotreload_resource(resource);
			had_hotreload = true;
		}
	}

	if (had_hotreload)
		printf("Hot-reloading finished (%llu ms)\n", watch.duration_ms());
}

void Resource::hotreload_resource(Resource* resource)
{
	printf("Hot-reloading '%s'\n", resource->path.data());
	resource->reload();
	resource->update_file_time();

	Array<Resource*> children_copy = resource->child_list;
	for(Resource* child : children_copy)
		hotreload_resource(child);
}

void Resource::add_dependency(Resource* resource)
{
	depend_list.add(resource);
	resource->child_list.add(this);
}

void Resource::clear_dependencies()
{
	for(Resource* res : depend_list)
		res->child_list.remove(this);

	depend_list.empty();
}

void Resource::update_file_time()
{
	file_time = get_file_modify_time(get_absolute_path().data());
}

String Resource::get_absolute_path()
{
	return Resource::resource_root + path;
}