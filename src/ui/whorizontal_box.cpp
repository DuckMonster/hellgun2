#include "whorizontal_box.h"
#include "debug/debug.h"

WIDGET_STYLE_IMPL(Horizontal_Box_Style);

UI_Rect WHorizontal_Box::add_child(Widget* child)
{
	children.add({ Horizontal_Box_Style::get(), child });
	return UI_Rect::zero;
}

void WHorizontal_Box::end()
{
	float x = 0.f;
	float max_y = 0.f;

	for(const auto& slot : children)
	{
		auto style = slot.style;
		Widget* widget = slot.widget;
		Vec2 padding = style._padding;

		widget->rect.position.x = x + padding.x;
		widget->rect.position.y = padding.y;

		Vec2 child_size = widget->rect.size + padding * 2.f;
		x += child_size.x;
		max_y = Math::max(max_y, child_size.y);
	}

	rect.size = Vec2(x, max_y);

	// Clear this cached data
	Horizontal_Box_Style::pop();
	children.empty();
}