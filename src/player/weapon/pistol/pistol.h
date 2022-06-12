#pragma once
#include "player/weapon/weapon.h"

class Pistol : public Weapon
{
public:
	void on_equipped() override;

	void update() override;
	void render(const Render_Info& info) override;

	int ammo = 6;
	Vec3 position;

	Vec3 recoil_offset;
	float recoil_angle;
};