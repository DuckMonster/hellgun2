#include "wimage.h"
#include "debug/debug.h"

void WImage::render(UI_Drawer& drawer)
{
	drawer.texture(UI_Rect(Vec2::zero, size), texture);
}