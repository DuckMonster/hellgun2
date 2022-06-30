#pragma once
#include "widget.h"
#include "widget_style.h"

struct Vertical_Box_Style
{
	WIDGET_STYLE_DECL(Vertical_Box_Style)

	WIDGET_STYLE_PROP(Vec2, padding)
};

class WVertical_Box : public Slotted_Widget<Vertical_Box_Style>
{
	WIDGET_CLASS(WVertical_Box)

public:
	void init() {}
	Vec2 get_desired_size() override;

	void render(UI_Drawer& drawer) override;
};