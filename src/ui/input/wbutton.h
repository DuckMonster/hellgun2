#pragma once
#include "ui/widget.h"
#include "core/delegate.h"

using Button_Callback = TDelegate<void>;

class WButton : public Slotted_Widget<Default_Style>
{
	WIDGET_CLASS(WButton)

public:
	void init() { delegate.clear(); }
	void init(const Button_Callback& in_delegate) { delegate = move(in_delegate); }

	// Helper for binding methods :)
	template<typename TObject>
	void init(TObject* obj, void (TObject::*func)())
	{
		delegate.bind_method(obj, func);
	}
	void on_end() override { pressed_last_frame = false; }

	void on_mouse_enter() override;
	void on_mouse_leave() override;
	void on_mouse_down() override;
	void on_mouse_up() override;

	bool was_pressed() { return pressed_last_frame; }
	bool is_held() { return _is_held; }
	bool is_hovered() { return _is_hovered; }

	Vec2 get_desired_size() override;
	void build(const UI_Rect& geom) override;
	void render(UI_Drawer& drawer) override;

private:
	Button_Callback delegate;
	bool _is_hovered = false;
	bool _is_held = false;

	// Used for inlined use with 'was_pressed()'
	bool pressed_last_frame = false;
};