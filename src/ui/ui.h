#pragma once
#include "wcanvas.h"
#include "ui_drawer.h"
#include "ui_rect.h"
#include "gfx/render_info.h"
#include "core/delegate.h"
#include "container/array.h"

class UI
{
public:
	using Capture_Delegate = Delegate<void, Widget*>;

	void init();
	void render(const Render_Info& info);

	void begin_frame();
	void end_frame();

	WCanvas* get_root() { return &root; }

	template<typename TWidget, typename... TArgs>
	TWidget* wadd(TArgs... args)
	{
		if (captures.count() == 0)
			error("wadd called with no captures active.\nYou have to call 'begin' somewhere first!");

		TWidget* widget = new TWidget();
		widget->init(args...);

		captures.top().execute(widget);
		return widget;
	}

	void push_capture(const Capture_Delegate& delegate);
	void pop_capture();

private:
	Array<Capture_Delegate> captures;
	UI_Drawer drawer;

	WCanvas root;
};

extern UI* ui;