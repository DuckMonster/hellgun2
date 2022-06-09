#include "trail_system.h"
#include "resource/resource.h"
#include "math/random.h"

void Trail_System::init(const Trail_Params& params)
{
	this->params = params;
	emitter = add_emitter<Emitter<Trail_Particle>>(Resource::load_material("material/fx/particle_trail.mat"));
	emitter->preserve_order = true;
	emitter->set_draw_mode(GL_LINE_STRIP);

	emitter->emit_particle(position, params.color, params.size, params.duration, { });
}

void Trail_System::system_update()
{
	if (time_has_reached(next_spawn_time))
	{
		emitter->emit_particle(position, params.color, params.size, params.duration, { });
		next_spawn_time = time_elapsed() + 1.f / params.frequency;
	}
}