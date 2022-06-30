#include "ui.h"
#include "resource/resource.h"
#include "core/context.h"
#include "wcanvas.h"

UI* ui;

void UI::init()
{
	drawer.init();
	root = new WCanvas();
}

void UI::new_frame()
{
	root->init();
	root->begin();
	current = root;
}

void UI::render(const Render_Info& info)
{
	root->end();
	drawer.begin(info);

	drawer.push_rect(UI_Rect(Vec2::zero, Vec2(context.width, context.height)));
	root->render(drawer);
	drawer.pop_rect();
}