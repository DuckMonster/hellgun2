#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>

void _messagebox(const char* title, const char* format, ...)
{
	String result;

	va_list vl;
	va_start(vl, format);

	// Measure the resulting size of the formatted string
	u32 length = vsnprintf(NULL, 0, format, vl);
	result.resize(length);

	// Actually format the thing
	vsprintf_s(result.data(), result.capacity(), format, vl);

	va_end(vl);

	MessageBoxA(NULL, result.data(), title, MB_OK);
}

void _errorexit() { exit(1); }
bool _can_debug_break() { return IsDebuggerPresent(); }