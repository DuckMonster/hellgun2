#include "ui_debug.h"
#include "resource/resource.h"
#include "gfx/mesh.h"
#include "gfx/material.h"

UI_Debug* ui_debug;

namespace
{
	Mesh rect_mesh;
	Material* debug_mat;
}

void UI_Debug::init()
{
	Vec2 rect_data[] =
	{
		Vec2(0.f, 0.f),
		Vec2(1.f, 0.f),
		Vec2(1.f, 1.f),
		Vec2(0.f, 1.f)
	};

	rect_mesh.init();
	rect_mesh.add_buffer(0);
	rect_mesh.buffer_data(0, sizeof(rect_data), rect_data);
	rect_mesh.bind_attribute(0, 0, 2, sizeof(Vec2), 0);
	rect_mesh.draw_mode = GL_LINE_LOOP;
	rect_mesh.draw_num = 4;

	debug_mat = Resource::load_material("material/ui_debug.mat");
}

void UI_Debug::rect(const Render_Info& info, const UI_Rect& rect)
{
	debug_mat->use();
	debug_mat->set("u_Min", rect.min);
	debug_mat->set("u_Max", rect.max);
	debug_mat->set("u_ViewProjection", info.ui_canvas);

	rect_mesh.draw();
}