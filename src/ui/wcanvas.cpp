#include "wcanvas.h"
#include "debug/debug.h"

WIDGET_STYLE_IMPL(Canvas_Style)

void WCanvas::render(UI_Drawer& drawer)
{
	Vec2 my_size = drawer.current_rect().size;

	for(const auto& slot : children)
	{
		Vec2 position = slot.style._position;
		Vec2 child_size = slot.widget->get_desired_size();

		position += my_size * slot.style._anchor;
		position -= child_size * slot.style._alignment;

		drawer.push_rect(UI_Rect(position, child_size));
		slot.widget->render(drawer);
		drawer.pop_rect();
	}
}