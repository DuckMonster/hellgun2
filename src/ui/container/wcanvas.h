#pragma once
#include "ui/widget.h"

struct Canvas_Style
{
	WIDGET_STYLE_DECL(Canvas_Style)
	WIDGET_STYLE_PROP(Vec2, alignment)
	WIDGET_STYLE_PROP(Vec2, anchor)
	WIDGET_STYLE_PROP(Vec2, position)
};

class WCanvas : public Slotted_Widget<Canvas_Style>
{
	WIDGET_CLASS(WCanvas)

public:
	void init() {}
	void build(const UI_Rect& geom) override;
};