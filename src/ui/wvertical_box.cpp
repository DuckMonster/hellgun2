#include "wvertical_box.h"

WIDGET_STYLE_IMPL(Vertical_Box_Style);

Vec2 WVertical_Box::get_desired_size()
{
	Vec2 size = Vec2::zero;
	for(auto& child : children)
	{
		Vec2 child_size = child.widget->get_desired_size() + child.style._padding * 2.f;

		size.x = Math::max(child_size.x, size.x);
		size.y += child_size.y;
	}

	return size;
}

void WVertical_Box::render(UI_Drawer& drawer)
{
	float y = 0.f;
	for(const auto& slot : children)
	{
		auto style = slot.style;
		Widget* widget = slot.widget;
		Vec2 child_size = widget->get_desired_size();
		Vec2 padding = style._padding;

		drawer.push_rect(UI_Rect(Vec2(padding.x, y + padding.y), child_size));
		widget->render(drawer);
		drawer.pop_rect();

		y += child_size.y + padding.y * 2.f;
	}
}