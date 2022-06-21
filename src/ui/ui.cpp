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

	root.render(drawer);

	drawer.pop_bounding_rect();
}

void UI::begin_frame()
{
	root.clear_children();
}

void UI::end_frame()
{
	if (captures.count() != 0)
		error("UI ended frame with non-zero pending captures.\nDid you forget an 'end' somewhere?");

	captures.empty();
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