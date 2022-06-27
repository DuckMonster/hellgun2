#include "ui.h"
#include "resource/resource.h"
#include "core/context.h"
#include "wcanvas.h"

UI* ui;

void UI::init()
{
	drawer.init();
	root = new Widget_Tree();
}

void UI::new_frame()
{
	current = root;
}

void UI::render(const Render_Info& info)
{
	/*
	root.build(UI_Rect(0.f, 0.f, context.width, context.height));

	drawer.begin(info);
	render_recursive(root);
	root.clear();
	*/
}