#pragma once
#include "entity/entity.h"

#define ENEMY_SPD (5.f)
#define ENEMY_ACC (15.f)
#define ENEMY_FRICTION (0.5f)

class Collider;

class Enemy : public Entity
{
	ENTITY_CLASS(Enemy)

public:
	float acceleration;
	float friction;

	Vec3 velocity;
	Vec3 target_offset;
	float offset_timer = 0.f;

	Collider* collider = nullptr;

	Vec3 jaw_location = Vec3::zero;

	void init() override;
	void on_destroyed() override;
	void update() override;
	void render(const Render_Info& info) override;

	void select_new_target_offset();
};