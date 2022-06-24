#pragma once

class Heap_Allocator
{
public:
	static void* malloc(u32 size);
	template<typename T>
	static T* malloc(u32 size)
	{
		return (T*)malloc(size);
	}

	static void free(void* ptr);

	template<typename T, typename... TArgs>
	static T* malloc_new(const TArgs&... args)
	{
		T* ptr = malloc<T>(sizeof(T));
		new(ptr) T(args...);

		return ptr;
	}

	template<typename T>
	static void free_delete(T* ptr)
	{
		ptr->~T();
		free(ptr);
	}

	// Debugging variables
	static u32 total_allocation_count;
	static u32 total_allocation_size;

	static void new_frame();
	static u32 frame_allocation_count;
	static u32 frame_allocation_delta;
	static u32 frame_allocation_size;
};
