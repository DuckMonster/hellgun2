#pragma once
#include "container/array.h"
#include "gfx/render_info.h"

class Particle_System;
class Fx
{
public:
	Array<Particle_System*> systems;

	template<typename T, typename... TArgs>
	T* spawn_system(const Vec3& position, const TArgs&... args)
	{
		T* system = new T();
		system->position = position;
		system->spawn_time = time_elapsed();
		system->init(args...);
		systems.add(system);

		return system;
	}

	void update();
	void render(const Render_Info& info);
};
extern Fx* fx;