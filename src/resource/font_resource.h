#pragma once
#include "resource.h"
#include "gfx/grid_font.h"

class Font_Resource : public Resource
{
public:
	Font_Resource(const String& path) : Resource(path) {}

	void init() override;
	void load() override;
	void reload() override { load(); }
	void free() override;

	Grid_Font font;
};