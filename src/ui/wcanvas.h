#pragma once
#include "widget.h"
#include "container/array.h"

struct Canvas_Slot_Info
{
	Vec2 position = Vec2::zero;

	Vec2 anchor = Vec2::zero;
	Vec2 alignment = Vec2::zero;
};

class WCanvas : public Widget
{
public:
	void begin(const Canvas_Slot_Info& info);
	void end();

	void render(UI_Drawer& drawer) override;
	void clear_children()
	{
		children.empty();
	}

private:
	Canvas_Slot_Info pending_slot;

	struct Child
	{
		Canvas_Slot_Info slot;
		Widget* widget;
	};
	Array<Child> children;
};