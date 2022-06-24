#pragma once
#include "widget.h"
#include "container/array.h"

class WCanvas : public Widget
{
public:
	void init() {}
	void build(Widget_Tree* tree, const UI_Rect& alotted_rect) override;
};