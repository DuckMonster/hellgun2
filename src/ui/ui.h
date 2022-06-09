#pragma once
#include "gfx/render_info.h"

class UI
{
public:
	void render(const Render_Info& info);
};
extern UI* ui;