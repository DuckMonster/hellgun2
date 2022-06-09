#pragma once
#include "gfx/render_info.h"

class Weapon
{
public:
	virtual void init() {}
	virtual void on_equipped() {}
	virtual void on_unequipped() {}

	virtual void update() {}
	virtual void render(const Render_Info& info) {}
};