#pragma once
#include "entity/entity.h"
#include "collision/collision.h"
#include "container/array.h"

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
#define PLAYER_JMP_IMPULSE (50.f)
#define PLAYER_AIR_JMP_IMPULSE (50.f)

// DASHING
#define PLAYER_GROUND_DASH_IMPULSE (50.f)
#define PLAYER_GROUND_POUND_IMPULSE (50.f)

// HEALTH
#define PLAYER_HIT_IMPULSE (80.f)
#define PLAYER_IMMUNE_TIME (2.f)

class Weapon;
class Drawable;

class Player : public Entity
{
	ENTITY_CLASS(Player)

public:
	void init() override;
	void on_destroyed() override;

	void update() override;
	bool is_alive() { return health > 0; }

	float get_movement_input();

	// Rendering
	Drawable* mesh = nullptr;
	Drawable* crosshair_circle;
	Drawable* crosshair_line;

	// Health
	int health = 3;
	float immune_until = 0.f;
	void hit(const Vec3& direction);
	bool is_immune() { return !time_has_reached(immune_until); }

	// Weapon stuff
	Array<Weapon*> weapons;
	u32 equipped_weapon = 0;

	// Movement stuff
	void update_movement();
	void update_grounded();
	void update_airborne();

	void move(Vec3 delta);
	Hit_Result sweep(const Vec3& delta);

	Vec3 velocity;
	bool is_grounded = false;
	int air_jumps = 0;

	float friction_scale = 1.f;
};