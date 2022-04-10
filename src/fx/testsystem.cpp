#include "testsystem.h"
#include "math/random.h"

void Test_System::init()
{
	emitter.init();
}

void Test_System::update()
{
	emitter.update();

	spawn_timer -= time_delta();
	if (spawn_timer <= 0.f)
	{
		for(int i = 0; i < 1; ++i)
		{
			emitter.emit_particle(
				Vec3::zero,
				Random::color(),
				Random::range(0.5f, 1.5f),
				{ Random::point_on_cone(Vec3::up, 90.f) * 10.f }
			);
		}

		spawn_timer = 1.f;
	}
}

void Test_System::render(const Render_Info& render_info)
{
	emitter.render(render_info);
}