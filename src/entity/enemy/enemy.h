#pragma once
#include "entity/entity.h"

#define ENEMY_SPD (5.f)

class Collider;

class Enemy : public Entity
{
	ENTITY_CLASS(Enemy)

public:
	Collider* collider = nullptr;

	void init() override;
	void on_destroyed() override;
	void update() override;
	void render(const Render_Info& info) override;
};