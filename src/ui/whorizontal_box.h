#pragma once
#include "widget.h"
#include "widget_style.h"

struct Horizontal_Box_Style
{
	WIDGET_STYLE_DECL(Horizontal_Box_Style)

	WIDGET_STYLE_PROP(float, valign)
	WIDGET_STYLE_PROP(Vec2, padding)
};

class WHorizontal_Box : public Slotted_Widget<Horizontal_Box_Style>
{
	WIDGET_CLASS(WHorizontal_Box)

public:
	void init() {}

	Vec2 get_desired_size() override;
	void build(const UI_Rect& geom) override;
};