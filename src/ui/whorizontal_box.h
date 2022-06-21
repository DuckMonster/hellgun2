#pragma once
#include "widget.h"

struct Horizontal_Box_Slot_Info
{
	Vec2 padding;
};

class WHorizontal_Box : public Widget
{
public:
	void init() {}
	Vec2 get_desired_size() override;

	void begin(const Horizontal_Box_Slot_Info& slot);
	void end();

	void render(UI_Drawer& drawer);

private:
	struct Child
	{
		Horizontal_Box_Slot_Info slot;
		Widget* widget;
	};

	Array<Child> children;
};