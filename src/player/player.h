#pragma once
#include "gfx/mesh.h"
#include "gfx/material.h"
#include "entity/entity.h"
#include "collision/collision.h"

#define PLAYER_SPD (25.f)

// AIRBORNE
#define PLAYER_AIR_FRICTION_H (3.2f)
#define PLAYER_AIR_FRICTION_V (0.7f)
#define PLAYER_AIR_ACC (PLAYER_SPD * PLAYER_AIR_FRICTION_H)

#define PLAYER_GRAVITY (90.f)

// GROUNDED
#define PLAYER_GROUND_FRICTION (9.8f)
#define PLAYER_GROUND_ACC (PLAYER_SPD * PLAYER_GROUND_FRICTION)

// JUMPING
#define PLAYER_JMP_IMPULSE (40.f)
#define PLAYER_AIR_JMP_IMPULSE (40.f)

// DASHING
#define PLAYER_GROUND_DASH_IMPULSE (50.f)

class Weapon;

class Player : public Entity
{
	ENTITY_CLASS(Player)

public:
	Vec3 velocity;

	void init() override;
	void update() override;
	void render(const Render_Info& info) override;

	Vec3 calculate_aim_position();
	float get_movement_input();

	// Weapon stuff
	Weapon* weapon = nullptr;

	// Movement stuff
	void update_movement();
	void update_grounded();
	void update_airborne();

	void move(Vec3 delta);
	Hit_Result sweep(const Vec3& delta);

	bool is_grounded = false;
	int air_jumps = 0;

	float friction_scale = 1.f;
};