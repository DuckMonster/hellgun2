#pragma once
#include "gfx/render_info.h"
#include "ui_rect.h"

class UI_Debug
{
public:
	void init();
	void rect(const Render_Info& info, const UI_Rect& rect);
};
extern UI_Debug* ui_debug;