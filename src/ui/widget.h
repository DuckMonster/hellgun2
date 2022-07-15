#pragma once
#include "ui.h"
#include "widget_style.h"
#include "gfx/render_info.h"
#include "ui_rect.h"
#include "ui_drawer.h"
#include "widget_path.h"
#include "debug/debug.h"
#include "core/input.h"

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

enum class Widget_Visibility : u8
{
	Visible,
	Hidden,
	Collapsed
};

struct Widget_Hit_Result
{
	Widget* widget = nullptr;
	UI_Rect rect = UI_Rect::zero;

	static Widget_Hit_Result select(const Widget_Hit_Result& a, const Widget_Hit_Result& b)
	{
		if (b.widget == nullptr)
			return a;

		return b;
	}
};

class Widget
{
public:
	Widget_ID id;
	UI_Rect rect = UI_Rect::zero;
	Input_Group input_group;

	Widget_Visibility visibility = Widget_Visibility::Visible;
	bool should_build() { return visibility != Widget_Visibility::Collapsed; }
	bool should_render() { return visibility == Widget_Visibility::Visible; }

	virtual ~Widget() {}
	virtual Widget_Class* get_class() = 0;

	// DONT OVERRIDE THESE
	virtual void begin() { fatal("Tried to begin widget '%s', but it doesn't support children", get_class()->name); }
	virtual void end() { fatal("Tried to end widget '%s', but it doesn't support children", get_class()->name); }
	virtual void on_begin() {}
	virtual void on_end() {}

	virtual void free() {}
	virtual void build(const UI_Rect& geom) {}

	// void init(); <-- NEEDED IN EVERY WIDGET CLASS (arguments optional)
	virtual void on_free() {}

	virtual Widget* get_or_add_child_cls(Widget_Class* cls) { return nullptr; }
	template<typename TWidget>
	TWidget* get_or_add_child()
	{
		return (TWidget*)get_or_add_child_cls(TWidget::static_class());
	}

	// If we GET to this point as a leaf widget, just return ourself
	virtual Widget_Hit_Result get_child_at(const Vec2& position, const UI_Rect& rect) { return { this, rect }; }
	virtual void build_path_to_position(Widget_Path& path, const Vec2& position, const UI_Rect& rect) {}

	virtual Vec2 get_desired_size() { return Vec2::zero; }
	virtual void render(UI_Drawer& drawer) {}

	// Various events!
	virtual void on_mouse_enter() {}
	virtual void on_mouse_leave() {}
	virtual void on_mouse_down() {}
	virtual void on_mouse_up() {}
};

template<typename TStyle>
class Slotted_Widget : public Widget
{
public:
	void begin() final
	{
		next_child_idx = 0;
		TStyle::push();

		on_begin();
	}

	void end() final
	{
		TStyle::pop();
		set_child_count(next_child_idx);

		on_end();
	}

	void free() override
	{
		// Free all children
		set_child_count(0);
	}

	Vec2 get_desired_size() override
	{
		Vec2 size = Vec2::zero;
		for(auto& child : children)
		{
			if (!child.widget->should_build())
				continue;

			size = component_max(size, child.widget->get_desired_size());
		}

		return size;
	}

	Widget* get_or_add_child_cls(Widget_Class* cls) override
	{
		Widget_ID id = Widget_ID(cls, next_child_idx);
		next_child_idx++;

		Widget* widget = find_child(id);
		if (widget)
			return widget;

		// Welp, seems we don't have this child....
		// In the case that this was an ID mismatch, set the child count
		//		to remove everything past this point.
		set_child_count(next_child_idx - 1);

		debug->print(TString::printf("Widget '%s' created", cls->name), 2.f);
		widget = cls->create_new();
		widget->id = id;
		add_child(widget);

		return widget;
	}

	void add_child(Widget* child) { children.add({ TStyle::get(), child }); }
	Widget* find_child(const Widget_ID& id)
	{
		if (children.count() <= id.number)
			return nullptr;

		if (children[id.number].widget->id != id)
			return nullptr;

		return children[id.number].widget;
	}
	void set_child_count(u32 new_count)
	{
		for(u32 i = new_count; i < children.count(); ++i)
		{
			debug->print(TString::printf("Widget '%s' destroyed", children[i].widget->get_class()->name), 2.f);
			ui->pre_destroy_widget(children[i].widget);

			children[i].widget->free();
			delete children[i].widget;
		}

		children.set_count(new_count);
	}

	Widget_Hit_Result get_child_at(const Vec2& position, const UI_Rect& rect) override
	{
		Widget_Hit_Result result = { this, rect };
		for(auto& slot : children)
		{
			UI_Rect child_rect = rect.transform(slot.rect);
			if (!child_rect.contains_point(position))
				continue;

			Widget_Hit_Result child_hit = slot.widget->get_child_at(position, child_rect);
			result = Widget_Hit_Result::select(result, child_hit);
		}

		return result;
	}
	void build_path_to_position(Widget_Path& path, const Vec2& position, const UI_Rect& rect) override
	{
		for(auto& slot : children)
		{
			if (!input->is_group_active(slot.widget->input_group))
				continue;

			UI_Rect child_rect = rect.transform(slot.rect);
			if (!child_rect.contains_point(position))
				continue;

			path.add(slot.widget);
			slot.widget->build_path_to_position(path, position, child_rect);
			return;
		}
	}

	// Default build if you don't care about style
	void build(const UI_Rect& geom) override
	{
		for(auto& slot : children)
		{
			if (!slot.widget->should_build())
				continue;

			slot.rect = UI_Rect(Vec2::zero, geom.size);
			slot.widget->build(slot.rect);
		}
	}

	// Default render if you have nothing to render yourself
	void render(UI_Drawer& drawer) override
	{
		for(auto& slot : children)
		{
			if (!slot.widget->should_render())
				continue;

			drawer.push_rect(slot.rect);
			slot.widget->render(drawer);
			drawer.pop_rect();
		}
	}

protected:
	struct Slot
	{
		TStyle style;
		Widget* widget;
		UI_Rect rect;
	};

	Array<Slot> children;
	u32 next_child_idx = 0;
};