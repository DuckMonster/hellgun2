#pragma once
#include "entity/entity.h"

class Cross;

#define CROSS_THROW_IMPULSE (45.f)
#define CROSS_FRICTION (0.25f)
#define CROSS_PULLBACK_STR (50.f)
#define CROSS_CATCH_IMPULSE (45.f)
#define CROSS_SWEEP_FREQUENCY (8.f)
#define CROSS_SWEEP_INTERVAL (1.f / CROSS_SWEEP_FREQUENCY)
#define CROSS_ROTATE_SPEED (25.f)

class Cross_Projectile : public Entity
{
	ENTITY_CLASS(Cross_Projectile)

public:
	Cross* host;
	Vec3 velocity;
	Vec3 last_sweep_position;
	float next_sweep_time;

	void init() override;
	void update() override;
	void render(const Render_Info& info) override;
};