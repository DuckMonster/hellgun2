#pragma once
#include "gfx/render_info.h"
#include "gfx/mesh.h"
#include "gfx/material.h"
#include "gfx/texture.h"
#include "ui_rect.h"

class Widget;

class UI_Drawer
{
public:
	void init();
	void begin(const Render_Info& info);

	void draw_rect(UI_Rect rect, const Color& clr);
	void fill_rect(UI_Rect rect, const Color& clr);
	void draw_texture(UI_Rect rect, Texture* texture);

	UI_Rect current_rect() { return rect_stack.top(); }
	Vec2 current_size() { return rect_stack.top().size; }
	void push_rect(const UI_Rect& new_rect)
	{
		if (rect_stack.count() == 0)
			rect_stack.add(new_rect);

		// Debug
		//draw_rect(new_rect, Color::white);
		rect_stack.add(current_rect().transform(new_rect));
	}
	void pop_rect()
	{
		rect_stack.pop();
	}

	Render_Info render_info;

private:
	Array<UI_Rect> rect_stack;

	// Straight rectangle, used for primitives
	Mesh rect_mesh;

	// Quad mesh, used for drawing textures
	Mesh quad_mesh;

	Material* primitive_material;
	Material* textured_material;
};