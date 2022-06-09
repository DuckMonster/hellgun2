#pragma once
#include "resource.h"
#include "import/dat.h"

class Dat_Resource : public Resource
{
public:
	Dat_Resource(const String& path) : Resource(path) {}

	void load() override;
	void reload() { load(); }
	void free() override;

	Dat_File dat;
};