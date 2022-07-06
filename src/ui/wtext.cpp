#include "wtext.h"

void WText::render(UI_Drawer& drawer)
{
	Grid_Font_Info font_info;
	font_info.background = Color::transparent;
	font_info.scale = 1.f;
	font->render_text(text, drawer.current_rect().position, font_info, drawer.render_info);
}