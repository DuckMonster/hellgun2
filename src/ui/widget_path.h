#pragma once

struct Widget_Path
{
	Array<Widget*> widgets;

	Widget_Path() {}
	Widget_Path(const Array<Widget*>& widgets) : widgets(widgets) {}
	Widget_Path(Array<Widget*>&& widgets) : widgets(widgets) {}

	void add(Widget* widget) { widgets.add(widget); }
	Widget* top() const
	{
		if (widgets.count() == 0)
			return nullptr;

		return widgets.top();
	}

	bool contains(Widget* other) const
	{
		return widgets.contains(other);
	}

	bool equals(const Widget_Path& other) const
	{
		return top() == other.top();
	}

	Widget_Path difference_from(const Widget_Path& other)
	{
		u32 branch_index = INDEX_NONE;
		for(u32 i = 0; i < widgets.count(); ++i)
		{
			// Either this path is longer than the other,
			//		or it contains another widget
			if (i >= other.widgets.count() || widgets[i] != other.widgets[i])
			{
				branch_index = i;
				break;
			}
		}

		// No difference, they are equal (as far as branching goes...)
		if (branch_index == INDEX_NONE)
			return Widget_Path();

		return Widget_Path(widgets.sub_array(branch_index, widgets.count() - branch_index));
	}

	void remove_branch(Widget* branch_root)
	{
		u32 branch_index = widgets.find(branch_root);

		// Uhm... It doesn't exist?
		if (branch_index == INDEX_NONE)
			return;

		widgets.set_count(branch_index);
	}

	// Lambda should be in form (Widget*) -> bool {}
	template<typename TLambda>
	void propegate(TLambda lambda)
	{
		for(i32 i = widgets.count() - 1; i >= 0; --i)
		{
			bool result = lambda(widgets[i]);
			if (result)
				return;
		}
	}
};