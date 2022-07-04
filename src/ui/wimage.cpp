#include "wimage.h"
#include "debug/debug.h"

void WImage::render(UI_Drawer& drawer)
{
	drawer.draw_texture(UI_Rect(Vec2::zero, size), texture);
}