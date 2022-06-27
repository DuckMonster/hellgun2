#pragma once
#include "widget.h"

class WImage : public Widget
{
	WIDGET_CLASS(WImage)

public:
	void init(const UI_Rect& geom, Texture* texture, const Vec2& size)
	{
		this->texture = texture;
		rect = UI_Rect(Vec2::zero, size);
	}

private:
	Texture* texture;
};