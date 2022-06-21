#pragma once
#include "gfx/render_info.h"
#include "ui_drawer.h"

class Widget
{
public:
	virtual Vec2 get_desired_size() { return Vec2::zero; }
	virtual void render(UI_Drawer& drawer) = 0;
};