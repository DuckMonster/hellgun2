#pragma once
#include "gfx/render_info.h"
#include "gfx/mesh.h"

class UI
{
public:
	void init();
	void render(const Render_Info& info);

	// Drawing methods


private:
	Mesh rect_mesh;
};
extern UI* ui;