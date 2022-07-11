#pragma once
#include "widget.h"
#include "widget_style.h"
#include "box_alignment.h"

struct Vertical_Box_Style
{
	WIDGET_STYLE_DECL(Vertical_Box_Style)

	WIDGET_STYLE_PROP(Vec2, padding)
	WIDGET_STYLE_PROP(Horizontal_Align, halign)
	WIDGET_STYLE_PROP(Vertical_Align, valign)
};

class WVertical_Box : public Slotted_Widget<Vertical_Box_Style>
{
	WIDGET_CLASS(WVertical_Box)

public:
	void init() {}
	Vec2 get_desired_size() override;

	void build(const UI_Rect& geom) override;
};