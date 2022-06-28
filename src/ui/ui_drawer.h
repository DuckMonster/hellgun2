#pragma once
#include "gfx/render_info.h"
#include "gfx/mesh.h"
#include "gfx/material.h"
#include "gfx/texture.h"
#include "core/context.h"
#include "ui_rect.h"

class Widget;

class UI_Drawer
{
public:
	enum class Action_Type : u8
	{
		Draw_Only,
		Draw_Push,
		Pop
	};

	struct Action
	{
		Action_Type type;
		Widget* widget;

		Action(Action_Type type, Widget* widget)
			: type(type), widget(widget) {}
	};

	void init();
	void render(const Render_Info& info);
	void clear_actions();

	void rect(const UI_Rect& rect);
	void texture(const UI_Rect& rect, Texture* texture);

	void add_action(Action_Type type, Widget* widget);
	UI_Rect get_bounding_rect()
	{
		if (rect_stack.count() == 0)
			return UI_Rect(Vec2::zero, Vec2(context.width, context.height));

		UI_Rect result = rect_stack[0];
		for(u32 i = 1; i < rect_stack.count(); ++i)
			result = result.transform(rect_stack[i]);

		return result;
	}

private:
	Array<Action> action_list;
	Array<UI_Rect> rect_stack;

	// Straight rectangle, used for primitives
	Mesh rect_mesh;

	// Quad mesh, used for drawing textures
	Mesh quad_mesh;

	Material* primitive_material;
	Material* textured_material;
};