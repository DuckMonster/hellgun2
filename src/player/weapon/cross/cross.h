#pragma once
#include "player/weapon/weapon.h"

class Cross : public Weapon
{
public:
	void on_equipped() override;

	void update() override;
	void render(const Render_Info& info) override;

	int ammo = 1;
	Vec3 position;
};