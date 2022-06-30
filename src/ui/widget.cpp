#include "widget.h"
#include "debug/debug.h"

void Widget::begin()
{
	next_child_idx = 0;
	on_begin();
}

void Widget::end()
{
	// Remove all children that weren't added this frame
	set_child_count(next_child_idx);
	on_end();
}

Widget* Widget::get_or_add_child(Widget_Class* cls)
{
	Widget_ID id = Widget_ID(cls, next_child_idx);
	next_child_idx++;

	Widget* widget = find_child(id);
	if (widget)
		return widget;

	// Welp, seems we don't have this child....
	// In the case that this was an ID mismatch, set the child count
	//		to remove everything past this point.
	set_child_count(next_child_idx - 1);

	debug->print(TString::printf("Widget '%s' created", cls->name), 2.f);
	widget = cls->create_new();
	widget->id = id;
	widget->parent = this;
	add_child(widget);

	return widget;
}