#pragma once
#include "gfx/renderinfo.h"
#include "emitter.h"

struct Test_Particle
{
	Vec3 velocity;
	void update(Particle_Vertex& vertex)
	{
		vertex.position += velocity * time_delta();
	}
};

class Test_System
{
public:
	Emitter<Test_Particle> emitter;
	float spawn_timer = 0.f;

	void init();
	void update();
	void render(const Render_Info& render_info);
};