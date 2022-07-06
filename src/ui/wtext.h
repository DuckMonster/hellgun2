#pragma once
#include "widget.h"
#include "gfx/grid_font.h"
#include "resource/resource.h"

class WText : public Widget
{
	WIDGET_CLASS(WText)

public:
	void init(const TString& text)
	{
		this->text = text;
		this->font = Resource::load_font("ui/font.dat");
	}
	void init(const TString& text, Grid_Font* font)
	{
		this->text = text; 
		this->font = font;
	}

	Vec2 get_desired_size() override
	{
		return font->measure_text(text);
	}
	void render(UI_Drawer& drawer) override;

private:
	TString text;
	Grid_Font* font;
};