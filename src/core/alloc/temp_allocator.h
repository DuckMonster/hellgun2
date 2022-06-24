#pragma once

class Temp_Allocator
{
public:
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
	static void ensure_capacity(u32 size);

	// Debugging variables
	static void new_frame();
	static u32 frame_allocation_count;
	static u32 frame_allocation_size;
};