#pragma once
#include "ui_drawer.h"
#include "ui_rect.h"
#include "gfx/render_info.h"
#include "container/array.h"
#include "widget_tree.h"
#include "widget.h"

class UI
{
public:
	void init();
	void new_frame();
	void render(const Render_Info& info);

	template<typename TWidget, typename... TArgs>
	bool begin(const TArgs&... args)
	{
		TWidget* widget = new TWidget();
		widget->init(args...);

		Widget_Tree* tree = tree_push();
		tree->widget = widget;
		tree->style = pending_style;

		style_stack.add(pending_style);
		pending_style = UI_Style();
		return true;
	}

	template<typename TWidget, typename... TArgs>
	void add(const TArgs&... args)
	{
		TWidget* widget = new TWidget();
		widget->init(args...);

		Widget_Tree* tree = tree_add();
		tree->widget = widget;
		tree->style = pending_style;
	}

	void end()
	{
		Widget_Tree* top = tree_pop();
		pending_style = style_stack.pop_copy();
	}

	// Styling functions
	void anchor(float x, float y) { pending_style.anchor = Vec2(x, y); }
	void anchor(const Vec2& anchor) { pending_style.anchor = anchor; }
	void alignment(const Vec2& alignment) { pending_style.alignment = alignment; }
	void padding(const Vec2& padding) { pending_style.padding = padding; }
	void padding(float padding) { pending_style.padding = Vec2(padding); }

private:
	void render_recursive(const Widget_Tree& tree);

	Widget_Tree* tree_top()
	{
		if (tree_stack.count() == 0)
			return &root;

		return tree_stack.top();
	}

	Widget_Tree* tree_add()
	{
		Widget_Tree& tree = tree_top()->children.add_default();
		return &tree;
	}

	Widget_Tree* tree_push()
	{
		Widget_Tree& tree = tree_top()->children.add_default();
		tree_stack.add(&tree);

		return &tree;
	}

	Widget_Tree* tree_pop()
	{
		Widget_Tree* top = tree_top();
		tree_stack.pop();

		return top;
	}

	UI_Drawer drawer;

	Array<Widget*> widgets;
	Array<UI_Style> style_stack;
	UI_Style pending_style;

	Array<Widget_Tree*> tree_stack;
	Widget_Tree root;
};

extern UI* ui;