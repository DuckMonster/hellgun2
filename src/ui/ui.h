#pragma once
#include "wcanvas.h"
#include "ui_drawer.h"
#include "ui_rect.h"
#include "gfx/render_info.h"
#include "container/array.h"

class UI
{
public:
	void init();
	void render(const Render_Info& info);

	void new_frame();

	template<typename TWidget, typename... TArgs>
	bool begin()
	{
		return true;
	}

	template<typename TWidget, typename... TArgs>
	void add(TArgs... args)
	{
	}

	void end()
	{
	}

private:
	UI_Drawer drawer;
};

extern UI* ui;