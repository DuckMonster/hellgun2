#include "particle_system.h"
#include "emitter.h"
#include "entity/entity.h"

void Particle_System::update()
{
	if (attach_entity)
		position = attach_entity->position;

	if (!finished)
		system_update();

	for(auto* emitter : emitters)
		emitter->update();
}

bool Particle_System::should_be_destroyed()
{
	if (!finished)
		return false;

	for(auto* emitter : emitters)
	{
		if (emitter->num_particles() > 0)
			return false;
	}

	return true;
}

void Particle_System::render(const Render_Info& info)
{
	for(auto* emitter : emitters)
		emitter->render(info);

	system_render(info);
}