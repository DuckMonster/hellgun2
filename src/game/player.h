#pragma once
#include "gfx/mesh.h"
#include "gfx/material.h"
#include "entity.h"

#define PLAYER_WALK_ACC (50.f)
#define PLAYER_FRICTION (2.1f)
#define PLAYER_GRAVITY (50.f)

class Player : public Entity
{
public:
	Mesh mesh;
	Material mat;

	Vec3 velocity;

	void init() override;
	void update() override;
	void render(const Render_Info& info) override;
};