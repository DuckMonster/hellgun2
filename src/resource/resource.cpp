#include "resource.h"
#include "gfx/material.h"
#include "gfx/texture.h"
#include "import/tga.h"
#include <stdio.h>

Map<String, void*> Resource::resource_map;

void Resource::register_resource(const String& str, void* data)
{
	resource_map.add(str, data);
	printf("Resource '%s' loaded\n", str.data());
}

Material* Resource::load_material(const char* vert_path, const char* frag_path)
{
	String resource_name = String::printf("%s : %s", vert_path, frag_path);
	if (resource_map.contains(resource_name))
		return (Material*)resource_map[resource_name];

	Material* mat = new Material();
	mat->load_file(vert_path, frag_path);

	register_resource(resource_name, mat);
	return mat;
}

Material* Resource::load_material(const char* vert_path, const char* geom_path, const char* frag_path)
{
	String resource_name = String::printf("%s : %s : %s", vert_path, geom_path, frag_path);
	if (resource_map.contains(resource_name))
		return (Material*)resource_map[resource_name];

	Material* mat = new Material();
	mat->load_file(vert_path, geom_path, frag_path);

	register_resource(resource_name, mat);
	return mat;
}

Texture* Resource::load_texture(const char* path)
{
	String resource_name = path;
	if (resource_map.contains(resource_name))
		return (Texture*)resource_map[resource_name];

	// Load tga file
	Tga_File tga;
	tga.load(path);
	tga.flip_vertical();

	// Upload to texture
	Texture* tex = new Texture();
	tex->init();
	tex->load_data(tga.data, tga.width, tga.height, tga.channels);

	tga.free();
	register_resource(resource_name, tex);
	return tex;
}