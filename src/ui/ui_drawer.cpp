#include "ui_drawer.h"
#include "resource/resource.h"

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
	primitive_material->use();
	primitive_material->set("u_ViewProjection", info.ui_canvas);

	textured_material->use();
	textured_material->set("u_ViewProjection", info.ui_canvas);
}

void UI_Drawer::rect(const UI_Rect& rect)
{
	UI_Rect bounding_rect = get_bounding_rect();

	primitive_material->use();
	primitive_material->set("u_Model", Mat4(
		rect.size.x, 0.f, 0.f, 0.f,
		0.f, rect.size.y, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		rect.position.x + bounding_rect.position.x, rect.position.y + bounding_rect.position.y, 0.f, 1.f
	));

	rect_mesh.draw();
}

void UI_Drawer::texture(const UI_Rect& rect, Texture* texture)
{
	UI_Rect bounding_rect = get_bounding_rect();

	textured_material->use();
	textured_material->set("u_Model", Mat4(
		rect.size.x, 0.f, 0.f, 0.f,
		0.f, rect.size.y, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		rect.position.x + bounding_rect.position.x, rect.position.y + bounding_rect.position.y, 0.f, 1.f
	));

	texture->bind();
	quad_mesh.draw();
}

void UI_Drawer::push_bounding_rect(const UI_Rect& rect)
{
	UI_Rect current = get_bounding_rect();
	UI_Rect local = UI_Rect(current.position + rect.position, rect.position);

	this->rect(rect);

	bounding_stack.add(local);
}

void UI_Drawer::push_bounding_rect_abs(const UI_Rect& rect)
{
	bounding_stack.add(rect);

	this->rect(rect);
}

void UI_Drawer::pop_bounding_rect()
{
	bounding_stack.pop();
}