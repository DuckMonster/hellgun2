#include "wimage.h"
#include "debug/debug.h"

void WImage::build()
{
	bounds = UI_Rect(Vec2::zero, size);
}

void WImage::render(UI_Drawer& drawer)
{
}