#pragma once
#include "widget.h"

class WImage : public Widget
{
public:
	void init(Texture* texture, const Vec2& size) { this->texture = texture; this->size = size; }
	void build(Widget_Tree* tree, const UI_Rect& alotted_rect);

	void render(UI_Drawer& drawer) override;

private:
	Texture* texture;
	Vec2 size;
};