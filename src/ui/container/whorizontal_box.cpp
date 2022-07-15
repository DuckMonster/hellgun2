#include "whorizontal_box.h"
#include "debug/debug.h"

WIDGET_STYLE_IMPL(Horizontal_Box_Style);

Vec2 WHorizontal_Box::get_desired_size()
{
	Vec2 size = Vec2::zero;

	for(const auto& slot : children)
	{
		auto style = slot.style;
		Widget* widget = slot.widget;

		Vec2 child_size = widget->get_desired_size() + style._padding * 2.f;
		size.x += child_size.x;
		size.y = Math::max(size.y, child_size.y);
	}

	return size;
}

void WHorizontal_Box::build(const UI_Rect& geom)
{
	Vec2 my_size = geom.size;

	// Calculate available y space for vertical filling
	float available_x = my_size.x - get_desired_size().x;
	if (available_x < 0.f)
		available_x = 0.f;

	// Count number of children who wants to fill
	u32 num_fills = 0;
	for(auto& slot : children)
	{
		if (slot.style._halign == Horizontal_Align::Fill ||
			slot.style._halign == Horizontal_Align::Center)
			num_fills++;
	}

	float fill_extra_x = available_x / num_fills;

	float x = 0.f;
	for(auto& slot : children)
	{
		auto style = slot.style;
		Widget* widget = slot.widget;
		Vec2 child_size = widget->get_desired_size();
		Vec2 padding = style._padding;
		Vec2 position = Vec2(x + padding.x, padding.y);

		// Vertical alignment
		float available_y = my_size.y - child_size.y - padding.y * 2.f;
		switch(style._valign)
		{
			case Vertical_Align::Center:
				position.y += available_y / 2.f;
				break;

			case Vertical_Align::Bottom:
				position.y += available_y;
				break;

			case Vertical_Align::Fill:
				child_size.y += available_y;
				break;
		}

		// Horizontal alignment
		switch(style._halign)
		{
			case Horizontal_Align::Fill:
				child_size.x += fill_extra_x;
				break;

			case Horizontal_Align::Center:
				position.x += fill_extra_x / 2.f;
				x += fill_extra_x / 2.f;
				break;
		}

		slot.rect = UI_Rect(position, child_size);
		slot.widget->build(slot.rect);

		x += child_size.x + padding.x * 2.f;
	}
}