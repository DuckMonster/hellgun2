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

void Temp_Allocator::init(u32 size)
{
	buffer = Heap_Allocator::malloc<u8>(size);
	buffer_size = size;
}

void* Temp_Allocator::malloc(u32 size)
{
	if (buffer_cursor + size > buffer_size)
		fatal("Temp buffer overflow (%u + %u)", buffer_cursor, size);

	void* ptr = buffer + buffer_cursor;
	buffer_cursor += size;

	frame_allocation_count++;
	frame_allocation_size += size;

	return ptr;
}