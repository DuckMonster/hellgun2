#pragma once
#include "gfx/render_info.h"
#include "ui_rect.h"
#include "ui_drawer.h"

struct Widget_Class
{
	const char* name;
	Widget_Class(const char* name) : name(name) {}
};

#define WIDGET_CLASS(cls_name)\
public:\
static Widget_Class* static_class()\
{\
	static Widget_Class cls(#cls_name);\
	return &cls;\
}\
Widget_Class* get_class() override\
{\
	return cls_name::static_class();\
}\
private:

struct Widget_ID
{
	Widget_Class* cls;
	u32 number;

	Widget_ID() : cls(nullptr), number(0) {}
	Widget_ID(Widget_Class* cls, u32 number) : cls(cls), number(number) {}

	bool operator==(const Widget_ID& other) { return cls == other.cls && number == other.number; }
	bool operator!=(const Widget_ID& other) { return cls != other.cls && number != other.number; }
};

class Widget
{
public:
	Widget_ID id;
	UI_Rect rect = UI_Rect::zero;
	virtual Widget_Class* get_class() = 0;

	// void init(); <-- NEEDED IN EVERY WIDGET CLASS (arguments optional)
	virtual void begin(const UI_Rect& geom) {}
	virtual UI_Rect add_child(Widget* child) { fatal("'%s' does not accept children", get_class()->name); return UI_Rect::zero; }
	virtual void end() {}

	virtual void render(UI_Drawer& drawer) {}
};