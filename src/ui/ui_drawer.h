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

	void rect(UI_Rect rect);
	void texture(UI_Rect rect, Texture* texture);

	UI_Rect current_rect() { return rect_stack.top(); }
	void push_rect(const UI_Rect& new_rect)
	{
		// Debug
		if (rect_stack.count() == 0)
			rect_stack.add(new_rect);

		rect(new_rect);
		rect_stack.add(current_rect().transform(new_rect));
	}
	void pop_rect()
	{
		rect_stack.pop();
	}

private:
	Array<UI_Rect> rect_stack;

	// Straight rectangle, used for primitives
	Mesh rect_mesh;

	// Quad mesh, used for drawing textures
	Mesh quad_mesh;

	Material* primitive_material;
	Material* textured_material;
};