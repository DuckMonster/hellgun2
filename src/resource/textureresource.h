#pragma once
#include "resource.h"
#include "gfx/texture.h"

class Texture_Resource : public Resource
{
public:
	Texture_Resource(const String& path) : Resource(path) {}

	void init() override;
	void load() override;
	void reload() override { load(); }
	void free() override;

	Texture texture;
};