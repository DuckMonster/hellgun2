#pragma once
#include "container/map.h"
#include "container/array.h"

struct Mesh;
struct Material;
struct Texture;
struct Shader;
struct Grid_Font;
struct Dat_File;
struct Level;

class Resource
{
protected:
	static String resource_root;
	static Map<String, void*> old_resource_map;
	static Map<String, Resource*> resource_map;
	static Array<Resource*> resources;

	static void register_resource(const String& str, void* data);

	template<typename ResourceT>
	static ResourceT* find_or_load_resource(const TString& path)
	{
		if (resource_map.contains(path))
			return (ResourceT*)resource_map[path];

		Stop_Watch watch;

		auto* resource = new ResourceT(path);
		resources.add(resource);
		resource_map.add(path, resource);

		resource->init();
		resource->load();
		resource->update_file_time();

		printf("Resource '%s' loaded (%llu ms)\n", path.data(), watch.duration_ms());
		return resource;
	}

public:
	static Mesh* load_mesh(const TString& path);
	static Material* load_material(const TString& path);
	static Shader* load_shader(const TString& path);
	static Texture* load_texture(const TString& path);
	static Level* load_level(const TString& path);
	static Grid_Font* load_font(const TString& path);

	static Dat_File* load_dat(const TString& path);

	static void update_hotreload();
	static void hotreload_resource(Resource* resource);

	Resource(const String& path) : path(path) {}

	virtual void init() {}
	virtual void load() {}
	virtual void reload() {}
	virtual void free() {}

	void add_dependency(Resource* resource);
	void clear_dependencies();

	void update_file_time();
	TString get_absolute_path();

	Array<Resource*> depend_list;
	Array<Resource*> child_list;

	const String path;
	u64 file_time;
};