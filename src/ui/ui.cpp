#include "ui.h"

UI* ui;

void UI::init()
{
	// Init rect mesh
	rect_mesh.init();
	rect_mesh.add_buffer(0);
	rect_mesh.bind_attribute(0, 0, 2);

	Vec2 rect_data[] = {
		Vec2(0.f, 0.f),
		Vec2(1.f, 0.f),
		Vec2(1.f, 1.f),
		Vec2(0.f, 1.f)
	};

	rect_mesh.buffer_data(0, sizeof(rect_data), rect_data);
	rect_mesh.draw_num = 4;
	rect_mesh.draw_mode = GL_LINE_LOOP;
}

void UI::render(const Render_Info& info)
{
	ui_debug->rect(info, UI_Rect(Vec2(16.f, 16.f), Vec2(64.f, 64.f)));
}