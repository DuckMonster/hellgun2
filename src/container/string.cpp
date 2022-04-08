#include "string.h"
#include <stdarg.h>
#include <stdio.h>

String String::printf(const char* format, ...)
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

	return result;
}