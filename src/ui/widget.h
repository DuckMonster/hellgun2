#pragma once
#include "gfx/renderinfo.h"

class Widget
{
public:
	void init();

	UI_Rect calculate_size();

	void render(const Render_Info& info);
};