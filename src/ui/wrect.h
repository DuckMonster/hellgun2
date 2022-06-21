#pragma once
#include "widget.h"

class WRect : public Widget
{
public:
	void init(const Vec2& size) { this->size = size; }
	Vec2 get_desired_size() override { return size; }

	void render(UI_Drawer& drawer);

	Vec2 size;
};