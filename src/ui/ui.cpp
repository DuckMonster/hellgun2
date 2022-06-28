#include "ui.h"
#include "resource/resource.h"
#include "core/context.h"
#include "wcanvas.h"

UI* ui;

void UI::init()
{
	drawer.init();
}

void UI::new_frame()
{
	next_widget_number = 0;
}

void UI::render(const Render_Info& info)
{
	/*
	root.build(UI_Rect(0.f, 0.f, context.width, context.height));

	drawer.begin(info);
	render_recursive(root);
	root.clear();
	*/

	if (widget_stack.count() != 0)
		error("widget_stack.count() > 0. You probably forgot to end() something!");

	drawer.render(info);
	drawer.clear_actions();

	widget_stack.empty();
}