#pragma once
#include "resource.h"
#include "game/level.h"

class Level_Resource : public Resource
{
public:
	Level_Resource(const String& path) : Resource(path) {}

	void init() override;
	void load() override;
	void reload() override { load(); }
	void free() override;

	Level level;
};