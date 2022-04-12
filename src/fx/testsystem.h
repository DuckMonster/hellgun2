#pragma once
#include "gfx/renderinfo.h"
#include "particlesystem.h"
#include "emitter.h"

class Test_System : public Particle_System
{
public:
	Simple_Emitter* tri_emitter;
	Simple_Emitter* star_emitter;

	float next_spawn_time = 0.f;

	void init();
	void system_update() override;
};