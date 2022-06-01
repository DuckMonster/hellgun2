#include "ui.h"
#include "ui_debug.h"

UI* ui;

void UI::render(const Render_Info& info)
{
	ui_debug->rect(info, UI_Rect(Vec2(16.f, 16.f), Vec2(64.f, 64.f)));
}