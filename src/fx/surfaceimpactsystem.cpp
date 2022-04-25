#include "surfaceimpactsystem.h"
#include "emitter.h"
#include "resource/resource.h"
#include "math/random.h"

void Surface_Impact_System::init(const Vec3& normal, const Vec3& enter_direction, const Vec3& exit_direction, float scale)
{
	Simple_Emitter* emitter = add_emitter<Simple_Emitter>(Resource::load_material("material/fx/particle_point.mat"));

	for(int i = 0; i < (int)(40 * scale); ++i)
	{
		Vec3 velocity =
			Random::point_on_circle(normal) * Random::range(65.f, 90.f) +
			normal * Random::range(-20.f, 20.f);

		emitter->emit_particle(
			position,
			Color::white,
			Random::range(0.2f, 0.4f),
			Random::range(0.3f, 0.5f),
			{
				velocity * scale,
				Vec3::up * Random::range(5.f, 30.f),
				Random::range(9.8f, 14.2f)
			}
		);
	}

	for(int i = 0; i < (int)(20 * scale); ++i)
	{
		emitter->emit_particle(
			position + Random::point_on_sphere(0.5f),
			Color::white,
			Random::range(0.2f, 0.4f),
			Random::range(0.5f, 0.9f),
			{
				Random::point_on_cone(exit_direction, 30.f) * Random::range(5.f, 75.f) * scale,
				Vec3::up * Random::range(5.f, 15.f),
				Random::range(4.5f, 6.2f)
			}
		);
	}

	finish_system();
}