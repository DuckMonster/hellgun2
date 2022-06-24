#include "widget_tree.h"
#include "widget.h"

void Widget_Tree::build(const UI_Rect& alotted_rect)
{
	if (!needs_rebuild)
		return;

	if (widget)
		widget->build(this, alotted_rect);
	else
		bounding_rect = alotted_rect;

	needs_rebuild = false;
	for(auto& child : children)
		child.build(bounding_rect);
}

void Widget_Tree::clear()
{
	children.empty();
	needs_rebuild = true;
}