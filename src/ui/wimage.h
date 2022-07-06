#pragma once
#include "widget.h"
#include "resource/resource.h"

class WImage : public Widget
{
	WIDGET_CLASS(WImage)

public:
	void init(const TString& texture_path, const Vec2& size)
	{
		this->texture = Resource::load_texture(texture_path);
		this->size = size;
	}
	void init(Texture* texture, const Vec2& size)
	{
		this->texture = texture;
		this->size = size;
	}

	Vec2 get_desired_size() override { return size; }
	void render(UI_Drawer& drawer) override;

private:
	Texture* texture;
	Vec2 size;
};