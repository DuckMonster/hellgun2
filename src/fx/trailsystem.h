#pragma once
#include "emitter.h"
#include "particlesystem.h"

struct Trail_Params
{
	float size = 1.f;
	float duration = 1.f;
	float frequency = 10.f;
	Color color = Color::white;
};

struct Trail_Particle
{
	Vec3 velocity;

	void update(const Particle_Update_Data& data)
	{
		if (data.index == data.emitter->num_particles() - 1)
		{
			data.vertex->position = data.system->position;
			return;
		}

		data.vertex->position += velocity * time_delta();
	}
};

class Trail_System : public Particle_System
{
public:
	Trail_Params params;
	Emitter<Trail_Particle>* emitter;

	float next_spawn_time = 0.f;

	void init(const Trail_Params& params);
	void system_update() override;
};