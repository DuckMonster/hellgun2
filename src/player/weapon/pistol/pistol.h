#pragma once
#include "player/weapon/weapon.h"

class Pistol : public Weapon
{
public:
	int ammo = 6;

	void update() override;
	void render(const Render_Info& info) override;
};