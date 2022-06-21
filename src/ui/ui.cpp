#include "ui.h"
#include "resource/resource.h"
#include "core/context.h"

UI* ui;

void UI::init()
{
	drawer.init();
}

void UI::render(const Render_Info& info)
{
	drawer.begin(info);
	drawer.push_bounding_rect_abs(UI_Rect(Vec2::zero, Vec2(context.width, context.height)));

	drawer.pop_bounding_rect();
}

void UI::new_frame()
{
}

void UI::push_capture(const Capture_Delegate& delegate)
{
	captures.emplace(move(delegate));
}

void UI::pop_capture()
{
	if (captures.count() == 0)
		error("pop_capture called with no pending captures.\nDid you accidentally call 'end' without a matching 'begin'?");

	captures.pop();
}