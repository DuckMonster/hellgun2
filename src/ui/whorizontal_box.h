#pragma once
#include "widget.h"
#include "widget_style.h"

struct Horizontal_Box_Style
{
	WIDGET_STYLE_DECL(Horizontal_Box_Style)

	float _valign = 0.f;
	static void valign(float valign) { get()._valign = valign; }

	WIDGET_STYLE_PROP(Vec2, padding)
};

class WHorizontal_Box : public Widget
{
	WIDGET_CLASS(WHorizontal_Box)

public:
	void init() {}

	void begin(const UI_Rect& geom) { rect = geom; Horizontal_Box_Style::push(); }
	UI_Rect add_child(Widget* child) override;
	void end() override;

private:
	struct Slot
	{
		Horizontal_Box_Style style;
		Widget* widget;
	};

	Array<Slot> children;
};