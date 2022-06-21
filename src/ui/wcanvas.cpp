#include "wcanvas.h"
#include "ui.h"

void WCanvas::begin(const Canvas_Slot_Info& info)
{
	pending_slot = info;
	ui->push_capture([this](Widget* widget)
	{
		children.add({pending_slot, widget});
	});
}

void WCanvas::end()
{
	ui->pop_capture();
}

void WCanvas::render(UI_Drawer& drawer)
{
	UI_Rect rect = drawer.get_bounding_rect();
	for(const auto& child : children)
	{
		UI_Rect child_rect;
		child_rect.size = child.widget->get_desired_size();

		// Position
		child_rect.position = child.slot.position;
		// Anchor
		child_rect.position += rect.size * child.slot.anchor;
		// Alignment
		child_rect.position -= child_rect.size * child.slot.alignment;

		drawer.push_bounding_rect(child_rect);
		child.widget->render(drawer);
	}
}