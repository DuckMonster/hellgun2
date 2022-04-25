#pragma once
#include "entity/entity.h"

class Trail_System;

class Pistol_Projectile : public Entity
{
	ENTITY_CLASS(Pistol_Projectile)

public:
	Vec3 velocity;
	Trail_System* trail;

	void init() override;
	void on_destroyed() override;

	void update() override;
	void render(const Render_Info& info) override;
};