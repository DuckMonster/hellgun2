#pragma once
#include "canvas.h"
#include "ui_rect.h"
#include "gfx/render_info.h"
#include "gfx/mesh.h"
#include "gfx/material.h"

class UI
{
public:
	void init();
	void render(const Render_Info& info);

	// Drawing methods
	void draw_rect(const Render_Info& info, const UI_Rect& rect);

	Canvas root;

private:
	Material* ui_material;
	Mesh rect_mesh;
};

extern UI* ui;