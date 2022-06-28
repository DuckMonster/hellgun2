#include "wcanvas.h"
#include "debug/debug.h"

WIDGET_STYLE_IMPL(Canvas_Style)

void WCanvas::begin(const UI_Rect& geom)
{
	rect = geom;
	Canvas_Style::push();
}

UI_Rect WCanvas::add_child(Widget* widget)
{
	children.add({ Canvas_Style::get(), widget });
	return UI_Rect::zero;
}

void WCanvas::end()
{
	Canvas_Style::pop();

	for(const auto& slot : children)
	{
		auto style = slot.style;
		Widget* widget = slot.widget;

		Vec2 child_size = widget->rect.size;
		Vec2 position = widget->rect.position;

		debug->print(TString::printf("anchor: {%f, %f}", style._anchor.x, style._anchor.y));

		position += rect.size * style._anchor;
		position -= child_size * style._alignment;

		widget->rect.position = position;
	}

	children.empty();
}