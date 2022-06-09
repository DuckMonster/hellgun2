#pragma once
#include "fx/particlesystem.h"

class Impact_Spike_System : public Particle_System
{
public:
	Vec3 velocity;
	float duration = 0.f;
	float scale = 1.f;

	void init(float scale, float duration, const Vec3& velocity);
	void system_update() override;
	void system_render(const Render_Info& info) override;
};