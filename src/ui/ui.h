#pragma once
#include "ui_drawer.h"
#include "ui_rect.h"
#include "widget_path.h"
#include "gfx/render_info.h"
#include "container/array.h"

class WCanvas;
class Widget;

class UI
{
public:
	void init();
	void new_frame();
	void end_frame();
	void render(const Render_Info& info);

	template<typename TWidget, typename... TArgs>
	TWidget* begin(const TArgs&... args)
	{
		TWidget* widget = current->get_or_add_child<TWidget>();
		widget->init(args...);
		widget->begin();

		current = widget;
		return widget;
	}

	template<typename TWidget, typename... TArgs>
	TWidget* add(const TArgs&... args)
	{
		TWidget* widget = current->get_or_add_child<TWidget>();
		widget->init(args...);
		return widget;
	}

	void end();
	void pre_destroy_widget(Widget* widget);

private:
	Widget_Path hovered_path;

	WCanvas* root = nullptr;
	Widget* current = nullptr;

	UI_Drawer drawer;
};

extern UI* ui;