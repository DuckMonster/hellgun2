#pragma once
#include "resource.h"
#include "gfx/shader.h"

class Shader_Resource : public Resource
{
public:
	Shader_Resource(const String& path) : Resource(path) {}

	void load();
	void reload() { load(); }
	void free();

	Shader shader;
};
