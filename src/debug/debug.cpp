#include "debug.h"
#include "gfx/mesh.h"
#include "gfx/material.h"
#include "gfx/gridfont.h"
#include "game/game.h"
#include "resource/resource.h"

Debug* debug;

namespace
{
	Material* debug_mat;
	Mesh line_mesh;
	Mesh point_mesh;
	Mesh box_mesh;
	Mesh sphere_mesh;
	Grid_Font debug_font;
}

void Debug::init()
{
	// Load material
	debug_mat = Resource::load_material("material/debug.mat");

	// Load line mesh
	const float line_data[] = {
		0.f, 0.f, 0.f,
		1.f, 0.f, 0.f
	};

	line_mesh.init();
	line_mesh.add_buffer(0);
	line_mesh.bind_attribute(0, 0, 3);
	line_mesh.buffer_data(0, sizeof(line_data), line_data);
	line_mesh.draw_mode = GL_LINES;
	line_mesh.draw_num = 2;

	// Load point mesh
	const float point_data[] = { 0.f, 0.f, 0.f };

	point_mesh.init();
	point_mesh.add_buffer(0);
	point_mesh.bind_attribute(0, 0, 3);
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
	box_mesh.bind_attribute(0, 0, 3);
	box_mesh.buffer_data(0, sizeof(box_data), box_data);

	box_mesh.add_element_buffer();
	box_mesh.element_data(sizeof(box_index_data), box_index_data);

	box_mesh.draw_mode = GL_LINES;
	box_mesh.draw_num = 4 * 2 * 3;

	// Load sphere mesh
	Array<Vec3> sphere_data;
	auto push_circle = [&sphere_data](const Vec3& normal) {
		Vec3 a = arbitrary_perpendicular(normal);
		a = normalize(a);
		Vec3 b = cross(a, normal);

		constexpr u32 resolution = 32; 
		constexpr float angle_step = TAU / resolution;

		for(u32 i = 0; i <= resolution; ++i)
		{
			float s_1 = Math::sin(angle_step * i);
			float s_2 = Math::sin(angle_step * (i + 1));
			float c_1 = Math::cos(angle_step * i);
			float c_2 = Math::cos(angle_step * (i + 1));

			sphere_data.emplace(a * s_1 + b * c_1);
			sphere_data.emplace(a * s_2 + b * c_2);
		}
	};

	push_circle(Vec3::forward);
	push_circle(Vec3::up);
	push_circle(Vec3::right);

	sphere_mesh.init();
	sphere_mesh.add_buffer(0);
	sphere_mesh.bind_attribute(0, 0, 3);
	sphere_mesh.buffer_data(0, sphere_data.count() * sizeof(Vec3), sphere_data.data());

	sphere_mesh.draw_mode = GL_LINES;
	sphere_mesh.draw_num = 33 * 2 * 3; // 33 * 2 lines for each circle, with 3 circles

	// Load font
	debug_font.load_file("res/font.tga", 6, 9);
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

void Debug::sphere(const Vec3& position, float radius, const Color& color, float thickness)
{
	Draw_Info& info = draw_list.add_default();
	info.mesh = &sphere_mesh;
	info.transform = mat_translation(position) * mat_scale(radius);
	info.color = color;
	info.thickness = thickness;
}

void Debug::text(const String& str, const Vec2& position, const Color& foreground, const Color& background, const Vec2& alignment)
{
	Text_Info& info = text_list.add_default();
	info.string = str;
	info.position = position;
	info.alignment = alignment;
	info.foreground = foreground;
	info.background = background;
}

void Debug::print(const String& str, float duration)
{
	Print_Info& info = print_list.add_default();
	info.string = str;
	info.print_time = time_elapsed();
	info.duration = duration;
}

void Debug::render(const Render_Info& info)
{
	// Draw list
	debug_mat->use();
	debug_mat->set("u_ViewProjection", info.view_projection);

	for(const auto& draw : draw_list)
	{
		debug_mat->set("u_Model", draw.transform);
		debug_mat->set("u_Color", draw.color);
		glLineWidth(draw.thickness);
		glPointSize(draw.thickness);

		draw.mesh->draw();
	}

	draw_list.empty();

	glDisable(GL_DEPTH_TEST);

	// Text list
	for(const auto& text : text_list)
	{
		Grid_Font_Info font_info;
		font_info.alignment = text.alignment;
		font_info.foreground = text.foreground;
		font_info.background = text.background;

		debug_font.render_text(text.string, text.position, font_info, info);
	}

	text_list.empty();

	// Print list
	float y = 10.f;
	for(u32 i = 0; i < print_list.count(); ++i)
	{
		debug_font.render_text(print_list[i].string, Vec2(10.f, y), Grid_Font_Info(), info);
		y += debug_font.glyph_h;

		// Check duration
		if (time_since(print_list[i].print_time) >= print_list[i].duration)
		{
			print_list.remove_at(i);
			i--;
		}
	}

	glEnable(GL_DEPTH_TEST);
}