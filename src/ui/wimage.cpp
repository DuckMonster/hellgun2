#include "wimage.h"
#include "debug/debug.h"

void WImage::build(Widget_Tree* tree, const UI_Rect& alotted_rect)
{
	debug->print("WImage::build");
	tree->bounding_rect = UI_Rect(Vec2::zero, size);
}

void WImage::render(UI_Drawer& drawer)
{
}