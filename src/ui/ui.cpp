#include "ui.h"
#include "resource/resource.h"

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

	ui_material = Resource::load_material("material/ui/ui.mat");
}

void UI::render(const Render_Info& info)
{
}

void UI::draw_rect(const Render_Info& info, const UI_Rect& rect)
{
	Mat4 model = Mat4(
		rect.size.x, 0.f, 0.f, 0.f,
		0.f, rect.size.y, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		rect.position.x, rect.position.y, 0.f, 1.f
	);

	ui_material->use();
	ui_material->set("u_Model", info.view_projection);
	ui_material->set("u_ViewProjection", model);

	rect_mesh.draw();
}