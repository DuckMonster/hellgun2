#pragma once
#include "player/weapon/weapon.h"

class Drawable;

class Pistol : public Weapon
{
public:
	void init() override;
	void on_equipped() override;
	void on_unequipped() override;

	void update() override;

	int ammo = 6;
	Vec3 position;

	Vec3 recoil_offset;
	float recoil_angle;

	Drawable* mesh;
};