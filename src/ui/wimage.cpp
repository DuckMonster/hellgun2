#include "wimage.h"

void WImage::render(UI_Drawer& drawer)
{
	drawer.texture(UI_Rect(Vec2::zero, size), texture);
}