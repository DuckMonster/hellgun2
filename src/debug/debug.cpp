#include "debug.h"
#include "gfx/mesh.h"
#include "gfx/material.h"
#include "game/game.h"

Debug debug;

namespace
{
	Material debug_mat;
	Mesh line_mesh;
	Mesh point_mesh;
	Mesh box_mesh;
}

void Debug::init()
{
	// Load material
	debug_mat.load_file("res/debug.vert", "res/debug.frag");

	// Load line mesh
	const float line_data[] = {
		0.f, 0.f, 0.f,
		1.f, 0.f, 0.f
	};

	line_mesh.init();
	line_mesh.add_buffer(0);
	line_mesh.bind_buffer(0, 3);
	line_mesh.buffer_data(0, sizeof(line_data), line_data);
	line_mesh.draw_mode = GL_LINES;
	line_mesh.draw_num = 2;

	// Load point mesh
	const float point_data[] = { 0.f, 0.f, 0.f };

	point_mesh.init();
	point_mesh.add_buffer(0);
	point_mesh.bind_buffer(0, 3);
	point_mesh.buffer_data(0, sizeof(point_data), point_data);
	point_mesh.draw_mode = GL_POINTS;
	point_mesh.draw_num = 1;

	// Load box mesh
	constexpr float u = 0.5f;
	const float box_data[] = {
		-u, -u, -u,
		u, -u, -u,
		u, u, -u,
		-u, u, -u,

		-u, -u, u,
		u, -u, u,
		u, u, u,
		-u, u, u,
	};

	const u32 box_index_data[] = {
		// Back face
		0, 1,	1, 2,	2, 3,	3, 0,
		// Front face
		4, 5,	5, 6,	6, 7,	7, 4,

		// Connecting lines
		0, 4,	1, 5,	2, 6,	3, 7,
	};

	box_mesh.init();
	box_mesh.add_buffer(0);
	box_mesh.bind_buffer(0, 3);
	box_mesh.buffer_data(0, sizeof(box_data), box_data);

	box_mesh.add_element_buffer();
	box_mesh.element_data(sizeof(box_index_data), box_index_data);

	box_mesh.draw_mode = GL_LINES;
	box_mesh.draw_num = 4 * 2 * 3;
}

void Debug::line(const Vec3& from, const Vec3& to, const Color& color, float thickness)
{
	Mat4 model = Mat4(to - from, Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), from);

	Draw_Info& info = draw_list.add_default();
	info.mesh = &line_mesh;
	info.transform = model;
	info.color = color;
	info.thickness = thickness;
}

void Debug::point(const Vec3& position, const Color& color, float size)
{
	Draw_Info& info = draw_list.add_default();
	info.mesh = &point_mesh;
	info.transform = mat_translation(position);
	info.color = color;
	info.thickness = size;
}

void Debug::vector(const Vec3& origin, const Vec3& vec, const Color& color, float thickness, float point_size)
{
	line(origin, origin + vec, color, thickness);
	point(origin + vec, color, point_size);
}

void Debug::box(const Mat4& transform, const Color& color, float thickness)
{
	Draw_Info& info = draw_list.add_default();
	info.mesh = &box_mesh;
	info.transform = transform;
	info.color = color;
	info.thickness = thickness;
}

void Debug::box(const Vec3& position, const Vec3& scale, const Quat& rotation, const Color& color, float thickness)
{
	box(mat_translation(position) * rotation.matrix() * mat_scale(scale), color, thickness);
}

void Debug::render(const Render_Info& info)
{
	debug_mat.use();
	debug_mat.set("u_ViewProjection", info.view_projection);

	for(const auto& info : draw_list)
	{
		debug_mat.set("u_Model", info.transform);
		debug_mat.set("u_Color", info.color);
		//glLineWidth(info.thickness);
		glPointSize(info.thickness);

		info.mesh->draw();
	}

	draw_list.empty();
}