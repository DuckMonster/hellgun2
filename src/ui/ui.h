#pragma once
#include "ui_drawer.h"
#include "ui_rect.h"
#include "widget_path.h"
#include "gfx/render_info.h"
#include "container/array.h"

class WCanvas;
class Widget;

struct Widget_Capture
{
	Widget* widget = nullptr;

	Widget_Capture(Widget* widget)
		: widget(widget) {}
};

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
		TWidget* widget = current_capture_widget()->get_or_add_child<TWidget>();
		capture_stack.emplace(widget);

		widget->input_group = input->get_active_group();
		widget->init(args...);
		widget->begin();

		return widget;
	}
	// Don't use this unless you know what you're doing...
	void begin_existing(Widget* widget);
	void end();

	template<typename TWidget, typename... TArgs>
	TWidget* add(const TArgs&... args)
	{
		TWidget* widget = current_capture_widget()->get_or_add_child<TWidget>();
		widget->init(args...);
		return widget;
	}

	void pre_destroy_widget(Widget* widget);

	Widget* current_capture_widget() { return capture_stack.top().widget; }
	bool has_capture_widget() { return capture_stack.count() > 0; }

private:
	WCanvas* root = nullptr;
	Array<Widget_Capture> capture_stack;

	UI_Drawer drawer;

	// Input stuff
	Widget_Path hovered_path;
};

extern UI* ui;