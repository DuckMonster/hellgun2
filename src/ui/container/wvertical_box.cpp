#include "wvertical_box.h"

WIDGET_STYLE_IMPL(Vertical_Box_Style);

Vec2 WVertical_Box::get_desired_size()
{
	Vec2 size = Vec2::zero;
	for(auto& child : children)
	{
		if (!child.widget->should_build())
			continue;

		Vec2 child_size = child.widget->get_desired_size() + child.style._padding * 2.f;

		size.x = Math::max(child_size.x, size.x);
		size.y += child_size.y;
	}

	return size;
}

void WVertical_Box::build(const UI_Rect& geom)
{
	Vec2 my_size = geom.size;

	// Calculate available y space for vertical filling
	float available_y = my_size.y - get_desired_size().y;
	if (available_y < 0.f)
		available_y = 0.f;

	// Count number of children who wants to fill
	u32 num_fills = 0;
	for(auto& slot : children)
	{
		if (slot.style._valign == Vertical_Align::Fill)
			num_fills++;
	}

	float fill_extra_y = available_y / num_fills;

	float y = 0.f;
	for(auto& slot : children)
	{
		if (!slot.widget->should_build())
			continue;

		auto style = slot.style;
		Widget* widget = slot.widget;
		Vec2 padding = style._padding;
		Vec2 child_size = widget->get_desired_size();
		Vec2 position = Vec2(padding.x, y + padding.y);

		// Horizontal alignment
		float available_x = my_size.x - child_size.x - padding.x * 2.f;
		switch(style._halign)
		{
			case Horizontal_Align::Left: break;
			case Horizontal_Align::Center: position.x += available_x * 0.5f; break;
			case Horizontal_Align::Right: position.x += available_x; break;
			case Horizontal_Align::Fill: child_size.x += available_x; break;
		}

		// Vertical alignment
		switch(style._valign)
		{
			case Vertical_Align::Top:
			case Vertical_Align::Bottom:
				break;

			case Vertical_Align::Fill:
				child_size.y += fill_extra_y;
				break;

			case Vertical_Align::Center:
				position.y += fill_extra_y / 2.f;
				y += fill_extra_y / 2.f;
				break;
		}

		slot.rect = UI_Rect(position, child_size);
		slot.widget->build(slot.rect);

		y += child_size.y + padding.y * 2.f;
	}
}