#pragma once

#define WIDGET_STYLE_DECL(type)\
static Array<type> style_stack;\
static type& get() {\
	if (style_stack.count() == 0)\
		fatal("Tried to get style " #type ", but such a style was not pushed.");\
	return style_stack.top();\
}\
static void push() { style_stack.add_default(); }\
static void pop() { style_stack.pop_copy(); }

#define WIDGET_STYLE_IMPL(type)\
Array<type> type::style_stack;

#define WIDGET_STYLE_PROP(type, name)\
type _##name;\
static void name(const type& name) { get()._##name = name; }