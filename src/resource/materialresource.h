#pragma once
#include "resource.h"
#include "gfx/material.h"
#include "import/dat.h"

class Material_Resource : public Resource
{
public:
	Material_Resource(const String& path) : Resource(path) {}
	void load() override;
	void reload() override { load(); }
	void free() override;

	Dat_File dat;
	Material material;
};