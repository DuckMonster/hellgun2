#include "resource.h"
#include "gfx/material.h"
#include "gfx/texture.h"
#include "core/io.h"
#include "import/tga.h"
#include "dat_resource.h"
#include "mesh_resource.h"
#include "shader_resource.h"
#include "material_resource.h"
#include "texture_resource.h"
#include "level_resource.h"
#include "font_resource.h"
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

Mesh* Resource::load_mesh(const TString& path)
{
	return &find_or_load_resource<Mesh_Resource>(path)->mesh;
}

Material* Resource::load_material(const TString& path)
{
	return &find_or_load_resource<Material_Resource>(path)->material;
}

Shader* Resource::load_shader(const TString& path)
{
	Shader_Resource* res = find_or_load_resource<Shader_Resource>(path);
	return &res->shader;
}

Texture* Resource::load_texture(const TString& path)
{
	return &find_or_load_resource<Texture_Resource>(path)->texture;
}

Level* Resource::load_level(const TString& path)
{
	return &find_or_load_resource<Level_Resource>(path)->level;
}

Grid_Font* Resource::load_font(const TString& path)
{
	return &find_or_load_resource<Font_Resource>(path)->font;
}

Dat_File* Resource::load_dat(const TString& path)
{
	Dat_Resource* res = find_or_load_resource<Dat_Resource>(path);
	return &res->dat;
}

void Resource::update_hotreload()
{
	bool had_hotreload = false;
	Stop_Watch watch;

	TArray<Resource*> resources_copy = resources;
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

TString Resource::get_absolute_path()
{
	return TString(Resource::resource_root) + path;
}