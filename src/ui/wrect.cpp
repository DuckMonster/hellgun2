#include "wrect.h"

void WRect::render(UI_Drawer& drawer)
{
	drawer.rect(UI_Rect(Vec2::zero, size));
}