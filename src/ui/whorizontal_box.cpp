#include "whorizontal_box.h"
#include "ui.h"

Vec2 WHorizontal_Box::get_desired_size()
{
	Vec2 size = Vec2::zero;
	for(const auto& child : children)
	{
		Vec2 padding = child.slot.padding;
		Vec2 child_size = child.widget->get_desired_size() + padding * 2.f;

		size.x += child_size.x;
		size.y = Math::max(size.y, child_size.y);
	}

	return size;
}

void WHorizontal_Box::begin(const Horizontal_Box_Slot_Info& slot)
{
	ui->push_capture([this, slot](Widget* widget) {
		children.add({ slot, widget });
	});
}
void WHorizontal_Box::end()
{
	ui->pop_capture();
}

void WHorizontal_Box::render(UI_Drawer& drawer)
{
	UI_Rect rect = drawer.get_bounding_rect();
	Vec2 position = Vec2::zero;

	for(const auto& child : children)
	{
		Vec2 padding = child.slot.padding;
		Vec2 child_size = child.widget->get_desired_size();
		UI_Rect child_rect = UI_Rect(position + padding, child_size);

		drawer.push_bounding_rect(child_rect);
		child.widget->render(drawer);
		drawer.pop_bounding_rect();

		position.x += child_size.x + padding.x * 2.f;
	}
}