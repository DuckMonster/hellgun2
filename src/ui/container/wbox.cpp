#include "wbox.h"

Vec2 WBox::get_desired_size()
{
	Vec2 max_size = size.is_set() ? *size : Vec2::zero;
	for(auto& slot : children)
	{
		if (!slot.widget->should_build())
			continue;

		max_size = component_max(max_size, slot.widget->get_desired_size() + slot.style._padding * 2.f);
	}

	return max_size;
}

void WBox::build(const UI_Rect& geom)
{
	for(auto& slot : children)
	{
		if (!slot.widget->should_build())
			continue;

		Vec2 padding = slot.style._padding;

		slot.rect = UI_Rect(padding, geom.size - padding * 2.f);
		slot.widget->build(slot.rect);
	}
}

void WBox::render(UI_Drawer& drawer)
{
	// Should we draw ourself?
	if (color.is_set())
	{
		// In that case, how big?
		drawer.fill_rect(UI_Rect(Vec2::zero, drawer.current_size()), *color);
	}

	// Draw children
	for(auto& slot : children)
	{
		if (!slot.widget->should_render())
			continue;

		drawer.push_rect(slot.rect);
		slot.widget->render(drawer);
		drawer.pop_rect();
	}
}