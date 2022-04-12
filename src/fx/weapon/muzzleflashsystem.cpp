#include "muzzleflashsystem.h"
#include "fx/emitter.h"
#include "resource/resource.h"
#include "math/random.h"

void Muzzle_Flash_System::init(const Vec3& direction)
{
	auto* point_emitter = add_emitter<Simple_Emitter>(Resource::load_material(
		"res/shader/fx/particle.vert",
		"res/shader/fx/particle_point.geom",
		"res/shader/fx/particle.frag"
	));

	for(int i = 0; i < 40; ++i)
	{
		point_emitter->emit_particle(
			position,
			Color::white,
			0.2f,
			Random::range(0.2f, 0.6f),
			{
				Random::point_on_cone(direction, 45.f) * Random::range(20.f, 80.f),
				Vec3::up * Random::range(15.f, 55.f),
				Random::range(4.2f, 5.2f)
			}
		);
	}

	finish_system();
}