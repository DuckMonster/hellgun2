#include "wbutton.h"

void WButton::on_mouse_enter() { _is_hovered = true; }
void WButton::on_mouse_leave()
{
	_is_hovered = false;
	_is_held = false;
}
void WButton::on_mouse_down() { _is_held = true; }
void WButton::on_mouse_up()
{
	// Ohh business time!
	if (_is_held)
	{
		pressed_last_frame = true;

		if (delegate.is_bound())
			delegate.execute();
	}

	_is_held = false;
}

Vec2 WButton::get_desired_size()
{
	Vec2 size = Vec2::zero;
	for(auto& slot : children)
	{
		Vec2 padding = slot.style._padding;
		Vec2 child_size = slot.widget->get_desired_size() + padding * 2.f;

		size = component_max(size, child_size);
	}

	return size;
}

void WButton::build(const UI_Rect& geom)
{
	Vec2 my_size = geom.size;
	for(auto& slot : children)
	{
		Vec2 padding = slot.style._padding;
		slot.rect.position = padding;
		slot.rect.size = my_size - padding * 2.f;

		slot.widget->build(slot.rect);
	}
}

void WButton::render(UI_Drawer& drawer)
{
	if (_is_hovered)
	{
		float alpha = _is_held ? 0.4f : 0.2f;
		drawer.fill_rect(UI_Rect(Vec2::zero, drawer.current_size()), Color(1.f, 1.f, 1.f, alpha));
	}

	for(auto& slot : children)
	{
		drawer.push_rect(slot.rect);
		slot.widget->render(drawer);
		drawer.pop_rect();
	}
}