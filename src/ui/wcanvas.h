#pragma once
#include "widget.h"
#include "widget_style.h"

struct Canvas_Style
{
	WIDGET_STYLE_DECL(Canvas_Style)
	WIDGET_STYLE_PROP(Vec2, alignment)
	WIDGET_STYLE_PROP(Vec2, anchor)
};

class WCanvas : public Widget
{
	WIDGET_CLASS(WCanvas)

public:
	void init() {}
	void begin(const UI_Rect& geom) override;
	UI_Rect add_child(Widget* widget) override;
	void end() override;

private:
	struct Slot
	{
		Canvas_Style style;
		Widget* widget;
	};

	Array<Slot> children;
};