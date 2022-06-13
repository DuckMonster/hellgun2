#pragma once
#include "player/weapon/weapon.h"

class Drawable;

class Cross : public Weapon
{
public:
	void init() override;
	void on_equipped() override;
	void on_unequipped() override;

	void update() override;

	int ammo = 1;
	Vec3 position;

	Drawable* mesh;
};