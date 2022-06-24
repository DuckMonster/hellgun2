#include "ui.h"
#include "resource/resource.h"
#include "core/context.h"

UI* ui;

void UI::init()
{
	drawer.init();
	root.widget = nullptr;
}

void UI::new_frame()
{
	root.clear();
}

void UI::render(const Render_Info& info)
{
	root.build(UI_Rect(0.f, 0.f, context.width, context.height));

	drawer.begin(info);
	render_recursive(root);
}

void UI::render_recursive(const Widget_Tree& tree)
{
	drawer.push_bounding_rect(tree.bounding_rect);

	if (tree.widget)
		tree.widget->render(drawer);

	for(const auto& child : tree.children)
		render_recursive(child);

	drawer.pop_bounding_rect();
}
