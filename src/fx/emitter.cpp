#include "emitter.h"
#include "resource/resourcecommon.h"
#include "container/array.h"
#include "math/random.h"

void Emitter::init()
{
	mesh.init();
	mesh.add_buffer(0);
	mesh.bind_attribute(0, 0, 3, sizeof(Particle), 0);
	mesh.bind_attribute(0, 1, 3, sizeof(Particle), 3 * sizeof(float));
	mesh.bind_attribute(0, 2, 4, sizeof(Particle), 6 * sizeof(float));
	mesh.bind_attribute(0, 3, 1, sizeof(Particle), 10 * sizeof(float));

	mesh.draw_mode = GL_POINTS;
	mat.load_file("res/shader/geom_test.vert", "res/shader/geom_test.geom", "res/shader/geom_test.frag");
}

void Emitter::update()
{
	age += time_delta();
	spawn_timer -= time_delta();
	if (spawn_timer <= 0.f)
	{
		particles.add({
			Vec3::zero,
			Random::point_on_cone(Vec3::up, 90.f) * Random::range(20.f, 30.f),
			Random::color(),
			age
		});
		mesh.buffer_data(0, particles.count() * sizeof(Particle), particles.data());
		mesh.draw_num = particles.count();

		spawn_timer = 0.1f;
	}
}

void Emitter::render(const Render_Info& render_info)
{
	mat.use();
	mat.set("u_ViewProjection", render_info.view_projection);
	mat.set("u_Model", mat_translation(position));
	mat.set("u_EmitterAge", age);
	mesh.draw();
}