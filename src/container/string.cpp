#include "string.h"
#include <stdarg.h>
#include <stdio.h>

template<typename TAllocator>
String_Base<TAllocator> String_Base<TAllocator>::printf(const char* format, ...)
{
	String_Base<TAllocator> result;

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

template String_Base<Heap_Allocator> String_Base<Heap_Allocator>::printf(const char* format, ...);
template String_Base<Temp_Allocator> String_Base<Temp_Allocator>::printf(const char* format, ...);