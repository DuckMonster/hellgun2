#pragma once
class Widget;

struct Widget_Tree
{
	Widget* widget = nullptr;
	Widget_Tree* parent = nullptr;

	template<typename TWidget, typename... TArgs>
	void add_child(const TArgs&... args)
	{
		Widget_ID id = Widget_ID(TWidget::static_class(), next_index);
		Widget_Tree* tree = find_or_add_child<TWidget>(id);
		TWidget* child_widget = (TWidget*)tree->widget;

		child_widget->init(args...);
	}

	template<typename TWidget, typename... TArgs>
	Widget_Tree* begin_child(const TArgs&... args)
	{
		Widget_ID id = Widget_ID(TWidget::static_class(), next_index);
		Widget_Tree* tree = find_or_add_child<TWidget>(id);
		TWidget* child_widget = (TWidget*)tree->widget;

		child_widget->init(args...);
		return tree;
	}

	Widget_Tree* end()
	{
		return parent;
	}

private:
	typename<typename TWidget>
	void find_or_add_child(const Widget_ID& id)
	{
		// Not enough cached children, it's new :)
		if (children.count() <= next_index)
		{
			Widget_Tree* tree = new Widget_Tree();
			tree->widget = new TWidget();

			children.add(tree);
			next_index++;
			return tree;
		}

		// It already exists! LUCKY!
		if (children[next_index]->widget->id == id)
		{
			return children[next_index++];
		}
		else
		{
			// Hmm, cache fail...
			// Delete all children after this one for now
			for(u32 i = next_index; i < children.count(); ++i)
			{
				children[i].free();
				delete children[i];
			}

			children.set_count(next_index);

			// Then add our new one
			Widget_Tree* tree = new Widget_Tree();
			tree->widget = new TWidget();

			children.add(tree);
			next_index++;
			return tree;
		}
	}

	void free()
	{
		delete widget;
		for(auto* child : children)
		{
			child->free();
			delete child;
		}

		children.empty();
	}

	Array<Widget_Tree*> children;
	Widget_Tree* parent;

	u32 next_index = 0;
};