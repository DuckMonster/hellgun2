#pragma once
#include "gfx/render_info.h"
#include "gfx/mesh.h"
#include "gfx/material.h"
#include "gfx/texture.h"
#include "ui_rect.h"

class UI_Drawer
{
public:
	void init();
	void begin(const Render_Info& info);

	void rect(const UI_Rect& rect);
	void texture(const UI_Rect& rect, Texture* texture);

	UI_Rect get_bounding_rect() { return bounding_stack.top(); }

	void push_bounding_rect(const UI_Rect& rect);
	void push_bounding_rect_abs(const UI_Rect& rect);
	void pop_bounding_rect();

private:
	Array<UI_Rect> bounding_stack;

	// Straight rectangle, used for primitives
	Mesh rect_mesh;

	// Quad mesh, used for drawing textures
	Mesh quad_mesh;

	Material* primitive_material;
	Material* textured_material;
};