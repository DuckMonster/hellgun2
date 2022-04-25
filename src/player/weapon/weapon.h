#pragma once
#include "gfx/renderinfo.h"

class Weapon
{
public:
	virtual void init() {}
	virtual void on_equipped() {}
	virtual void on_unequipped() {}

	virtual void update() {}
	virtual void render(const Render_Info& info) {}
};