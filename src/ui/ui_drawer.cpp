#include "ui_drawer.h"
#include "widget.h"
#include "resource/resource.h"
#include "debug/debug.h"

void UI_Drawer::init()
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

	// Init quad mesh
	quad_mesh.init();
	quad_mesh.add_buffer(0);
	quad_mesh.bind_attribute(0, 0, 2, sizeof(Vec2) * 2, 0); // Position
	quad_mesh.bind_attribute(0, 1, 2, sizeof(Vec2) * 2, sizeof(Vec2)); // UV
	Vec2 quad_data[] = {
		Vec2(0.f, 0.f), Vec2(0.f, 1.f),
		Vec2(1.f, 0.f), Vec2(1.f, 1.f),
		Vec2(1.f, 1.f), Vec2(1.f, 0.f),

		Vec2(0.f, 0.f), Vec2(0.f, 1.f),
		Vec2(1.f, 1.f), Vec2(1.f, 0.f),
		Vec2(0.f, 1.f), Vec2(0.f, 0.f),
	};
	quad_mesh.buffer_data(0, sizeof(quad_data), quad_data);
	quad_mesh.draw_num = 6;
	quad_mesh.draw_mode = GL_TRIANGLES;

	primitive_material = Resource::load_material("material/ui/primitive.mat");
	textured_material = Resource::load_material("material/ui/textured.mat");
}

void UI_Drawer::begin(const Render_Info& info)
{
	render_info = info;
	primitive_material->use();
	primitive_material->set("u_ViewProjection", info.ui_canvas);

	textured_material->use();
	textured_material->set("u_ViewProjection", info.ui_canvas);
}

void UI_Drawer::draw_rect(UI_Rect rect, const Color& clr)
{
	rect = current_rect().transform(rect);

	primitive_material->use();
	primitive_material->set("u_Model", Mat4(
		rect.size.x, 0.f, 0.f, 0.f,
		0.f, rect.size.y, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		rect.position.x, rect.position.y, 0.f, 1.f
	));
	primitive_material->set("u_Color", clr);

	rect_mesh.draw();
}

void UI_Drawer::fill_rect(UI_Rect rect, const Color& clr)
{
	rect = current_rect().transform(rect);

	primitive_material->use();
	primitive_material->set("u_Model", Mat4(
		rect.size.x, 0.f, 0.f, 0.f,
		0.f, rect.size.y, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		rect.position.x, rect.position.y, 0.f, 1.f
	));
	primitive_material->set("u_Color", clr);

	quad_mesh.draw();
}

void UI_Drawer::draw_texture(UI_Rect rect, Texture* texture)
{
	rect = current_rect().transform(rect);

	textured_material->use();
	textured_material->set("u_Model", Mat4(
		rect.size.x, 0.f, 0.f, 0.f,
		0.f, rect.size.y, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		rect.position.x, rect.position.y, 0.f, 1.f
	));

	texture->bind();
	quad_mesh.draw();
}