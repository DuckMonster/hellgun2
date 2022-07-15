#include "wcanvas.h"
#include "debug/debug.h"

WIDGET_STYLE_IMPL(Canvas_Style)

void WCanvas::build(const UI_Rect& geom)
{
	Vec2 my_size = geom.size;

	for(auto& slot : children)
	{
		Vec2 position = slot.style._position;
		Vec2 child_size = slot.widget->get_desired_size();

		position += my_size * slot.style._anchor;
		position -= child_size * slot.style._alignment;

		slot.rect = UI_Rect(position, child_size);
		slot.widget->build(slot.rect);
	}
}