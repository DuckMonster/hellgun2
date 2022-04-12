#pragma once
#include "entity/entity.h"

class Trail_System;

class Bullet : public Entity
{
	ENTITY_CLASS(Bullet)

public:
	Vec3 velocity;
	Trail_System* trail;

	void init() override;
	void on_destroyed() override;

	void update() override;
	void render(const Render_Info& info) override;
};