#include "testsystem.h"
#include "math/random.h"
#include "resource/resource.h"
#include "debug/debug.h"

void Test_System::init()
{
	tri_emitter = add_emitter<Simple_Emitter>(Resource::load_material("material/fx/particle_triangle.mat"));
	star_emitter = add_emitter<Simple_Emitter>(Resource::load_material("material/fx/particle_star.mat"));
}

void Test_System::system_update()
{
	if (time_has_reached(next_spawn_time))
	{
		tri_emitter->emit_particle(
			position,
			Random::color(),
			1.f,
			Random::range(0.5f, 1.5f),
			{ Random::point_on_sphere(10.f), Vec3::up * 50.f }
		);
		star_emitter->emit_particle(
			position,
			Random::color(),
			1.f,
			Random::range(2.5f, 4.8f),
			{ Random::point_on_sphere(10.f), Vec3::up * 20.f }
		);

		next_spawn_time = time_elapsed() + 0.1f;
	}

	if (get_age() > 3.f)
		finish_system();
}