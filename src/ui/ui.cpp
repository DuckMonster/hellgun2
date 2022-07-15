#include "ui.h"
#include "resource/resource.h"
#include "core/context.h"
#include "core/input.h"
#include "container/wcanvas.h"

UI* ui;

void UI::init()
{
	drawer.init();
	root = new WCanvas();
}

void UI::new_frame()
{
	// Begin the root
	capture_stack.emplace(root);
	root->begin();
}

void UI::end_frame()
{
	end();
	if (capture_stack.count() != 0)
		fatal("Ended the UI frame, but capture stack is not empty.\nYou probably forgot an 'end()' somwhere!");

	root->build(UI_Rect(Vec2::zero, Vec2(context.width, context.height)));
}

void UI::begin_existing(Widget* widget)
{
	capture_stack.emplace(widget);
	widget->begin();
}

void UI::end()
{
	if (!has_capture_widget())
		fatal("Tried to end widget, but we're currently at the root!\nYou probably have more end's than begin's");

	Widget* widget = current_capture_widget();
	capture_stack.pop();

	widget->end();
}

void UI::pre_destroy_widget(Widget* widget)
{
	// SO!
	// If we're destroying a widget, we need to clean up our trees
	hovered_path.remove_branch(widget);
}

void UI::render(const Render_Info& info)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	UI_Rect screen_rect = UI_Rect(Vec2::zero, Vec2(context.width, context.height));
	drawer.begin(info);
	drawer.push_rect(screen_rect);

	if (input->key_pressed(Key::F2))
		drawer.debug = !drawer.debug;

	root->render(drawer);

	// !! UPDATE HOVER PATH !!
	Widget_Path new_path;
	new_path.add(root);

	root->build_path_to_position(new_path, input->mouse_position(), UI_Rect(Vec2::zero, Vec2(context.width, context.height)));

	if (!hovered_path.equals(new_path))
	{
		Widget_Path enter_path = new_path.difference_from(hovered_path);
		enter_path.propegate([](Widget* widget) -> bool
		{
			widget->on_mouse_enter();
			return false;
		});

		Widget_Path exit_path = hovered_path.difference_from(new_path);
		exit_path.propegate([](Widget* widget) -> bool
		{
			widget->on_mouse_leave();
			return false;
		});

		hovered_path = new_path;
	}

	// !! MOUSE EVENTS !!
	if (input->mouse_pressed(Mouse_Btn::Left))
	{
		hovered_path.propegate([](Widget* widget) -> bool
		{
			widget->on_mouse_down();
			return false;
		});
	}
	if (input->mouse_released(Mouse_Btn::Left))
	{
		hovered_path.propegate([](Widget* widget) -> bool
		{
			widget->on_mouse_up();
			return false;
		});
	}

	drawer.pop_rect();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}