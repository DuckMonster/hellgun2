#pragma once
#include "gfx/render_info.h"
#include "ui_rect.h"

class Widget
{
public:
	void init();

	UI_Rect calculate_size();
	void render(const Render_Info& info);
};