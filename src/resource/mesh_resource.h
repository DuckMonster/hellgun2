#pragma once
#include "resource.h"
#include "gfx/mesh.h"

class Mesh_Resource : public Resource
{
public:
	Mesh_Resource(const String& path) : Resource(path) {}

	void init() override;
	void load() override;
	void reload() { load(); }
	void free() override;

	Mesh mesh;
};