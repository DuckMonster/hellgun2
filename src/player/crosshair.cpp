#include "crosshair.h"
#include "gfx/mesh.h"

Mesh* get_crosshair_circle()
{
	static Mesh mesh;
	static bool loaded = false;

	if (!loaded)
	{
		mesh.init();
		mesh.add_buffer(0);
		mesh.bind_attribute(0, 0, 2, 2 * sizeof(float), 0);

		constexpr u32 CIRCLE_RESOLUTION = 32;
		Vec2 circle_data[CIRCLE_RESOLUTION];
		float angle_step = TAU / CIRCLE_RESOLUTION;

		for(u32 i = 0; i < CIRCLE_RESOLUTION; ++i)
			circle_data[i] = Vec2(Math::cos(angle_step * i), Math::sin(angle_step * i));

		mesh.buffer_data(0, sizeof(circle_data), circle_data);
		mesh.draw_num = CIRCLE_RESOLUTION;
		mesh.draw_mode = GL_LINE_LOOP;
		loaded = true;
	}

	return &mesh;
}

Mesh* get_crosshair_line()
{
	static Mesh mesh;
	static bool loaded = false;

	if (!loaded)
	{
		Vec2 line_data[] = { Vec2(0.5f, 0.f), Vec2(1.f, 0.f) };
		float line_alphas[] = { 0.f, 1.f };

		mesh.init();
		mesh.add_buffer(0);
		mesh.bind_attribute(0, 0, 2, 2 * sizeof(float), 0);
		mesh.buffer_data(0, sizeof(line_data), line_data);

		mesh.add_buffer(1);
		mesh.bind_attribute(1, 1, 1, 1 * sizeof(float), 0);
		mesh.buffer_data(1, sizeof(line_alphas), line_alphas);

		mesh.draw_num = 2;
		mesh.draw_mode = GL_LINES;
		loaded = true;
	}

	return &mesh;
}