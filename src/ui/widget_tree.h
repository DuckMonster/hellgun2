#pragma once
#include "container/array.h"
#include "ui_rect.h"
#include "ui_style.h"

class Widget;

struct Widget_Tree
{
public:
	UI_Style style;

	Widget* widget;
	Array<Widget_Tree> children;

	UI_Rect bounding_rect;
	bool needs_rebuild = true;

	void build(const UI_Rect& alotted_rect);
	void clear();
};