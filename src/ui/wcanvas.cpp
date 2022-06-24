#include "wcanvas.h"
#include "debug/debug.h"

void WCanvas::build(Widget_Tree* tree, const UI_Rect& alotted_rect)
{
	tree->bounding_rect = alotted_rect;

	for(auto& child : tree->children)
	{
		// Build first to get the size
		child.build(UI_Rect::zero);

		const UI_Style& style = child.style;

		// Reposition child according to anchors and stuff
		Vec2 position = child.bounding_rect.position;

		// Anchor
		position += alotted_rect.size * style.anchor;
		// Alignment
		position -= child.bounding_rect.size * style.alignment;

		child.bounding_rect.position = position;
	}
}