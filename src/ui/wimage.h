#pragma once
#include "widget.h"

class WImage : public Widget
{
	WIDGET_CLASS(WImage)

public:
	void init(Texture* texture, const Vec2& size)
	{
		this->texture = texture;
		this->size = size;
	}

	void begin(const UI_Rect& geom) override
	{
		rect = UI_Rect(Vec2::zero, size);
	}

private:
	Texture* texture;
	Vec2 size;
};