#pragma once
#include "gfx/render_info.h"
#include "widget_tree.h"
#include "ui_drawer.h"

class Widget
{
public:
	virtual void build(Widget_Tree* tree, const UI_Rect& alotted_rect) {}
	virtual void render(UI_Drawer& drawer) {}
};