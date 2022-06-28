#pragma once
#include "core/context.h"
#include "ui_drawer.h"
#include "ui_rect.h"
#include "gfx/render_info.h"
#include "container/array.h"
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
		TWidget* widget = get_or_create_widget<TWidget>(args...);
		widget->init(args...);

		begin_widget(widget);
		drawer.add_action(UI_Drawer::Action_Type::Draw_Push, widget);

		return true;
	}

	template<typename TWidget, typename... TArgs>
	void add(const TArgs&... args)
	{
		TWidget* widget = get_or_create_widget<TWidget>(args...);
		widget->init(args...);

		begin_widget(widget);
		end_widget(widget);

		drawer.add_action(UI_Drawer::Action_Type::Draw_Only, widget);
	}

	void end()
	{
		Widget* top = widget_stack.top();
		end_widget(top);

		drawer.add_action(UI_Drawer::Action_Type::Pop, top);
	}

private:
	template<typename TWidget, typename... TArgs>
	TWidget* get_or_create_widget(const TArgs&... args)
	{
		Widget_ID id = Widget_ID(TWidget::static_class(), next_widget_number);

		// Not enough widgets created...
		if (widgets.count() <= next_widget_number)
			return create_widget<TWidget>(id, args...);

		// Otherwise, check the ID!
		if (widgets[next_widget_number]->id == id)
		{
			// Match!
			TWidget* widget = (TWidget*)widgets[next_widget_number++];
			widget->init(args...);

			return widget;
		}
		else
		{
			// Shit, ID mismatch
			// Remove everything for now
			for(u32 i = next_widget_number; i < widgets.count(); ++i)
				delete widgets[i];

			widgets.set_count(next_widget_number);
			return create_widget<TWidget>(id, args...);
		}
	}

	template<typename TWidget, typename... TArgs>
	TWidget* create_widget(const Widget_ID& id, const TArgs&... args)
	{
		TWidget* widget = new TWidget();
		widget->id = id;
		widget->init(args...);
		widgets.add(widget);

		next_widget_number++;
		return widget;
	}

	void begin_widget(Widget* widget)
	{
		// Fetch geometry from parent, if there is one available
		// Default
		UI_Rect geom = UI_Rect(Vec2::zero, Vec2(context.width, context.height));

		if (widget_stack.count() > 0)
			geom = widget_stack.top()->add_child(widget);

		widget->begin(geom);
		widget_stack.add(widget);
	}

	void end_widget(Widget* widget)
	{
		widget->end();
		widget_stack.pop();
	}

	Array<Widget*> widgets;
	u32 next_widget_number = 0;

	UI_Drawer drawer;
	Array<Widget*> widget_stack;
};

extern UI* ui;