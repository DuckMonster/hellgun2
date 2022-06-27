#pragma once
#include "ui_drawer.h"
#include "ui_rect.h"
#include "ui_style.h"
#include "gfx/render_info.h"
#include "container/array.h"
#include "widget.h"

class UI
{
public:
	void init();
	void new_frame();
	void render(const Render_Info& info);

	template<typename TWidget, typename... TArgs>
	bool begin(const TArgs&... args)
	{
		current = current->begin_child<TWidget>(args...);
	}

	template<typename TWidget, typename... TArgs>
	void add(const TArgs&... args)
	{
		current->add_child<TWidget>(args...);
	}

	void end()
	{
		current = current->end();
	}

	// Styling functions
	void anchor(float x, float y) { pending_style.anchor = Vec2(x, y); }
	void anchor(const Vec2& anchor) { pending_style.anchor = anchor; }
	void alignment(const Vec2& alignment) { pending_style.alignment = alignment; }
	void padding(const Vec2& padding) { pending_style.padding = padding; }
	void padding(float padding) { pending_style.padding = Vec2(padding); }

private:
	UI_Drawer drawer;
	Widget_Tree* root;
	Widget_Tree* current;

	Array<UI_Style> style_stack;
	UI_Style pending_style;

	u32 next_widget_number = 0;
};

extern UI* ui;