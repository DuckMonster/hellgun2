#pragma once

class Temp_Allocator
{
public:
	static void init(u32 size);
	static void* malloc(u32 size);
	template<typename T>
	static T* malloc(u32 size)
	{
		return (T*)malloc(size);
	}

	static void free(void* ptr) {}

	static u8* buffer;
	static u32 buffer_size;
	static u32 buffer_cursor;

	// Debugging variables
	static void new_frame();
	static u32 frame_allocation_count;
	static u32 frame_allocation_size;
};