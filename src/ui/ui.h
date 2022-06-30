#pragma once
#include "ui_drawer.h"
#include "ui_rect.h"
#include "gfx/render_info.h"
#include "container/array.h"
#include "widget.h"

class WCanvas;

class UI
{
public:
	void init();
	void new_frame();
	void render(const Render_Info& info);

	template<typename TWidget, typename... TArgs>
	bool begin(const TArgs&... args)
	{
		TWidget* widget = current->get_or_add_child<TWidget>();
		widget->init(args...);
		widget->begin();

		current = widget;
		return true;
	}

	template<typename TWidget, typename... TArgs>
	void add(const TArgs&... args)
	{
		TWidget* widget = current->get_or_add_child<TWidget>();
		widget->init(args...);

		// Just begin - end real quick :)
		widget->begin();
		widget->end();
	}

	void end()
	{
		current->end();
		current = current->parent;
	}

private:
	WCanvas* root = nullptr;
	Widget* current = nullptr;

	UI_Drawer drawer;
};

extern UI* ui;