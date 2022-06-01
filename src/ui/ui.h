#pragma once
#include "gfx/renderinfo.h"

class UI
{
public:
	void render(const Render_Info& info);
};
extern UI* ui;