#include "heap_allocator.h"
#include <stdlib.h>
#include <stdio.h>

u32 Heap_Allocator::total_allocation_count = 0;
u32 Heap_Allocator::total_allocation_size = 0;

void Heap_Allocator::new_frame()
{
	frame_allocation_count = frame_allocation_delta = frame_allocation_size = 0;
}
u32 Heap_Allocator::frame_allocation_count = 0;
u32 Heap_Allocator::frame_allocation_delta = 0;
u32 Heap_Allocator::frame_allocation_size = 0;

void* Heap_Allocator::malloc(u32 size)
{
	// Debugging
	total_allocation_size += size;
	total_allocation_count++;

	frame_allocation_count++;
	frame_allocation_delta++;
	frame_allocation_size += size;

	return ::malloc(size);
}

void Heap_Allocator::free(void* ptr)
{
	// Debugging
	total_allocation_size -= _msize(ptr);
	total_allocation_count--;

	frame_allocation_delta--;

	::free(ptr);
}

// Override new/delete to use our heap allocator :)
void* operator new(size_t size)
{
	return Heap_Allocator::malloc(size);
}

void operator delete(void* ptr)
{
	Heap_Allocator::free(ptr);
}