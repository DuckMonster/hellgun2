#pragma once
#include "widget.h"

class WCollapse_Tab : public Slotted_Widget<Default_Style>
{
	WIDGET_CLASS(WCollapse_Tab)

public:
	void init() {}

	void on_begin();
	void on_child_end();

	Vec2 get_desired_size() { return children[0].widget->get_desired_size(); }

private:
	bool collapsed = false;
};