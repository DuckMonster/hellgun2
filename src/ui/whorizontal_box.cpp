#include "whorizontal_box.h"
#include "ui.h"

void WHorizontal_Box::build()
{
	/*
	// First calculate total bounds for this horizontal box
	tree->bounding_rect = UI_Rect::zero;

	for(auto& child : tree->children)
	{
		child.build(UI_Rect::zero);
		Vec2 padding = child.style.padding;

		tree->bounding_rect.size.x += child.bounding_rect.size.x + padding.x * 2.f;
		tree->bounding_rect.size.y = Math::max(tree->bounding_rect.size.y, child.bounding_rect.size.y + padding.y * 2.f);
	}

	Vec2 my_size = tree->bounding_rect.size;

	// Start positioning the children
	float x = 0;

	for(auto& child : tree->children)
	{
		Vec2 padding = child.style.padding;
		Vec2 anchor = child.style.anchor;

		Vec2 child_size = child.bounding_rect.size + padding * 2.f;

		// Check for left-over space on the y-axis for anchoring
		float y_space = my_size.y - child_size.y;

		child.bounding_rect.position = Vec2(x + padding.x, padding.y + y_space * anchor.y);
		x += child_size.x;
	}
	*/
}