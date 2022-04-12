#pragma once
#include "container/map.h"

struct Material;
struct Texture;

class Resource
{
	static Map<String, void*> resource_map;
	static void register_resource(const String& str, void* data);

public:
	static Material* load_material(const char* vert_path, const char* frag_path);
	static Material* load_material(const char* vert_path, const char* geom_path, const char* frag_path);
	static Texture* load_texture(const char* path);
};