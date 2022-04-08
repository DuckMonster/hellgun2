#pragma once
#include "gfx/renderinfo.h"
#include "gfx/mesh.h"
#include "gfx/material.h"

class Emitter
{
	Mesh mesh;
	Material mat;

	#pragma pack(push, 1)
	struct Particle
	{
		Vec3 position;
		Vec3 initial_velocity;
		Color color;

		float spawn_time;
	};
	#pragma pack(pop)

	Array<Particle> particles;

public:
	Vec3 position;
	float age = 0.f;
	float spawn_timer = 0.f;

	void init();
	void update();
	void render(const Render_Info& render_info);
};