#pragma once
#include "widget.h"

class WHorizontal_Box : public Widget
{
public:
	void init() {}
	void build(Widget_Tree* tree, const UI_Rect& alotted_rect) override;
};