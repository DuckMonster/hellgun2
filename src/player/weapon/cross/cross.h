#pragma once
#include "player/weapon/weapon.h"

class Cross : public Weapon
{
public:
	void update() override;
	void render(const Render_Info& info) override;
};