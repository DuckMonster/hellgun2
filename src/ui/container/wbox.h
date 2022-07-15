#pragma once
#include "ui/widget.h"

class WBox : public Slotted_Widget<Default_Style>
{
	WIDGET_CLASS(WBox)

public:
	void init()
	{
		size.unset();
		color.unset();
	}
	void init(const Vec2& in_size)
	{
		size = in_size;
		color.unset();
	}
	void init(const Color& in_clr)
	{
		size.unset();
		color = in_clr;
	}
	void init(const Vec2& in_size, const Color& in_clr)
	{
		size = in_size;
		color = in_clr;
	}

	Vec2 get_desired_size() override;
	void build(const UI_Rect& geom) override;
	void render(UI_Drawer& drawer) override;

private:
	Optional<Vec2> size;
	Optional<Color> color;
};