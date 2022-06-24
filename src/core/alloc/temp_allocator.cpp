#include "temp_allocator.h"

u8* Temp_Allocator::buffer = nullptr;
u32 Temp_Allocator::buffer_size = 0;
u32 Temp_Allocator::buffer_cursor = 0;

void Temp_Allocator::new_frame()
{
	buffer_cursor = 0;
	frame_allocation_count = 0;
	frame_allocation_size = 0;
}
u32 Temp_Allocator::frame_allocation_count = 0;
u32 Temp_Allocator::frame_allocation_size = 0;

void* Temp_Allocator::malloc(u32 size)
{
	ensure_capacity(buffer_cursor + size);
	void* ptr = buffer + buffer_cursor;
	buffer_cursor += size;

	frame_allocation_count++;
	frame_allocation_size += size;

	return ptr;
}

void Temp_Allocator::ensure_capacity(u32 size)
{
	if (size <= buffer_size)
		return;

	size = Math::ceil_po2(size);
	u8* new_buffer = Heap_Allocator::malloc<u8>(size);

	if (buffer)
	{
		memcpy(new_buffer, buffer, buffer_cursor);
		Heap_Allocator::free(buffer);
	}

	buffer = new_buffer;
	buffer_size = size;
}
