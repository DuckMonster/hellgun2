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
	float x = 0.f;
	for(auto& slot : children)
	{
		auto style = slot.style;
		Widget* widget = slot.widget;
		Vec2 child_size = widget->get_desired_size();
		Vec2 padding = style._padding;

		slot.rect = UI_Rect(Vec2(x + padding.x, padding.y), child_size);
		slot.widget->build(slot.rect);

		x += child_size.x + padding.x * 2.f;
	}
}

void WHorizontal_Box::render(UI_Drawer& drawer)
{
	for(auto& slot : children)
	{
		drawer.push_rect(slot.rect);
		slot.widget->render(drawer);
		drawer.pop_rect();
	}
}