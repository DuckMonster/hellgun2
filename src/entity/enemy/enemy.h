#pragma once
#include "entity/entity.h"
#include "damage_data.h"

#define ENEMY_SPD (5.f)
#define ENEMY_ACC (15.f)
#define ENEMY_FRICTION (0.5f)

class Collider;
class Drawable;

class Enemy : public Entity
{
	ENTITY_CLASS(Enemy)

public:
	float acceleration;
	float friction;

	float health = 10.f;

	Vec3 velocity;
	Vec3 target_offset;
	float offset_timer = 0.f;

	Collider* collider = nullptr;

	Drawable* skull = nullptr;
	Drawable* jaw_front = nullptr;
	Drawable* jaw_back = nullptr;

	float mouth_angle = 0;
	float next_mouth_twitch_time = 0.f;

	void hit(const Damage_Data& data);

	void init() override;
	void on_destroyed() override;
	void update() override;

	void select_new_target_offset();
};