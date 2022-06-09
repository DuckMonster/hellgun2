#pragma once
#include "container/array.h"
#include "gfx/render_info.h"

class Emitter_Base;
class Entity;
struct Material;

class Particle_System
{
	Entity* attach_entity = nullptr;
	bool finished = false;

	Array<Emitter_Base*> emitters;

public:
	float spawn_time;
	float get_age() { return time_since(spawn_time); }

	Vec3 position = Vec3::zero;
	Quat rotation = Quat::identity;

	template<typename T, typename... TArgs>
	T* add_emitter(const TArgs... args)
	{
		T* emitter = new T();
		emitters.add(emitter);

		emitter->system = this;
		emitter->init(args...);
		return emitter;
	}

	void attach_to(Entity* entity) { attach_entity = entity; }
	void detach() { attach_entity = nullptr; }

	void update();
	virtual void system_update() { }

	void render(const Render_Info& info);
	virtual void system_render(const Render_Info& info) {}

	void finish_system() { finished = true; }
	bool is_finished() { return finished; }
	bool should_be_destroyed();
};