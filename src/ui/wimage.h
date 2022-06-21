#pragma once
#include "widget.h"

class WImage : public Widget
{
public:
	void init(const Vec2& size, Texture* texture) { this->size = size; this->texture = texture; }
	Vec2 get_desired_size() override { return size; }

	void render(UI_Drawer& drawer);

private:
	Texture* texture;
	Vec2 size;
};