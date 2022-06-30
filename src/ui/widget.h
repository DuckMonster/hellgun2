#pragma once
#include "gfx/render_info.h"
#include "ui_rect.h"
#include "ui_drawer.h"

struct Widget_Class
{
	const char* name;
	Widget_Class(const char* name) : name(name) {}

	virtual Widget* create_new() = 0;
};

template<typename TWidget>
struct Widget_Class_Impl : public Widget_Class
{
	Widget_Class_Impl(const char* name) : Widget_Class(name) {}
	Widget* create_new() override { return new TWidget(); }
};

#define WIDGET_CLASS(cls_name)\
public:\
static Widget_Class* static_class()\
{\
	static Widget_Class_Impl<cls_name> cls(#cls_name);\
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
	bool operator!=(const Widget_ID& other) { return cls != other.cls || number != other.number; }
};

class Widget
{
public:
	Widget_ID id;
	UI_Rect rect = UI_Rect::zero;
	Widget* parent = nullptr;

	virtual ~Widget() {}

	virtual Widget_Class* get_class() = 0;
	Widget* get_or_add_child(Widget_Class* cls);
	template<typename TWidget>
	TWidget* get_or_add_child()
	{
		return (TWidget*)get_or_add_child(TWidget::static_class());
	}

	// DONT OVERRIDE THESE
	// Use on_begin() and on_end() instead
	virtual void begin();
	virtual void end();

	virtual void free() {}

	// void init(); <-- NEEDED IN EVERY WIDGET CLASS (arguments optional)
	virtual void on_begin() {}
	virtual void on_end() {}
	virtual void on_free() {}

	virtual void add_child(Widget* child) { fatal("Widget '%s' does not accept children.", get_class()->name); }
	virtual Widget* find_child(const Widget_ID& id) { fatal("Widget '%s' does not accept children.", get_class()->name); return nullptr; }
	virtual void set_child_count(u32 new_count) { if (new_count > 0) fatal("Widget '%s' does not accept children.", get_class()->name); }

	virtual Vec2 get_desired_size() { return Vec2::zero; }

	virtual void render(UI_Drawer& drawer) {}

private:
	u32 next_child_idx = 0;
};

template<typename TStyle>
class Slotted_Widget : public Widget
{
public:
	void begin() final
	{
		TStyle::push();
		Widget::begin();
	}

	void end() final
	{
		TStyle::pop();
		Widget::end();
	}

	void free() override
	{
		for(auto& child : children)
		{
			child.widget->free();
			delete child.widget;
		}

		children.empty();
	}

	void add_child(Widget* child) final { children.add({ TStyle::get(), child }); }
	Widget* find_child(const Widget_ID& id) final
	{
		if (children.count() <= id.number)
			return nullptr;

		if (children[id.number].widget->id != id)
			return nullptr;

		return children[id.number].widget;
	}
	void set_child_count(u32 new_count) final
	{
		for(u32 i = new_count; i < children.count(); ++i)
		{
			children[i].widget->free();
			delete children[i].widget;
		}

		children.set_count(new_count);
	}

protected:
	struct Slot
	{
		TStyle style;
		Widget* widget;
	};

	Array<Slot> children;
};