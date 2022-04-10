#pragma once

void _messagebox(const char* title, const char* format, ...);
void _errorexit();
bool _can_debug_break();

#define DEBUG_BREAK (_can_debug_break() && (__debugbreak(), 1))
#define error(format, ...) (_messagebox("error triggered", format, __VA_ARGS__), DEBUG_BREAK)
#define fatal(format, ...) (_messagebox("fatal triggered", format, __VA_ARGS__), DEBUG_BREAK, _errorexit())