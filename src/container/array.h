#pragma once
#include <new>
#include <string.h>
#include "core/alloc/heap_allocator.h"
#include "core/alloc/temp_allocator.h"

#define INDEX_NONE (~0)

template<typename T, typename TAllocator>
struct Array_Base
{
	template<typename T, typename TAllocator>
	friend struct Array_Base;

public:
	struct Iterator
	{
		Iterator(Array_Base<T, TAllocator>& arr, int index) : arr(arr), index(index) {}

		Iterator& operator++() { index++; return *this; }
		Iterator& operator--() { index--; return *this; }
		T& operator*() { return arr[index]; }

		bool operator==(const Iterator& other) { return index == other.index; }
		bool operator!=(const Iterator& other) { return index != other.index; }

		Array_Base<T, TAllocator>& arr;
		int index = 0;
	};

	struct Const_Iterator
	{
		Const_Iterator(const Array_Base<T, TAllocator>& arr, int index) : arr(arr), index(index) {}

		Const_Iterator& operator++() { index++; return *this; }
		Const_Iterator& operator--() { index--; return *this; }
		const T& operator*() { return arr[index]; }

		bool operator==(const Const_Iterator& other) { return index == other.index; }
		bool operator!=(const Const_Iterator& other) { return index != other.index; }

		const Array_Base<T, TAllocator>& arr;
		int index = 0;
	};

	Array_Base() {}
	Array_Base(const Array_Base& other)
	{
		_data = TAllocator::malloc<T>(other.data_size());
		_count = other._count;
		_capacity = other._capacity;

		for(u32 i = 0; i < _count; ++i)
			new(_data + i) T(other[i]);
	}
	template<typename TOtherAllocator>
	Array_Base(const Array_Base<T, TOtherAllocator>& other)
	{
		_data = TAllocator::malloc<T>(other.data_size());
		_count = other._count;
		_capacity = other._capacity;

		for(u32 i = 0; i < _count; ++i)
			new(_data + i) T(other[i]);
	}
	Array_Base(Array_Base&& other)
	{
		_data = other._data;
		_count = other._count;
		_capacity = other._capacity;

		other._data = nullptr;
		other._count = 0;
		other._capacity = 0;
	}
	~Array_Base()
	{
		for(u32 i = 0; i < _count; ++i)
			_data[i].~T();

		if (_data)
			TAllocator::free(_data);
	}

	u32 capacity() const { return _capacity; }
	u32 count() const { return _count; }
	T* data() { return _data; }
	const T* data() const { return _data; }

	u32 data_size() const { return sizeof(T) * _count; }

	T& operator[](u32 index)
	{
		if (index >= _count)
			fatal("Array index %d is out-of-bounds", index);

		return _data[index];
	}
	const T& operator[](u32 index) const
	{
		if (index >= _count)
			fatal("Array index %d is out-of-bounds", index);

		return _data[index];
	}

	void operator=(const Array_Base& other)
	{
		empty();
		ensure_capacity(other._capacity);

		// Copy over each element
		for(u32 i = 0; i < other._count; ++i)
			new(_data + i) T(other[i]);

		_count = other._count;
	}
	template<typename TOtherAllocator>
	void operator=(const Array_Base<T, TOtherAllocator>& other)
	{
		empty();
		ensure_capacity(other._capacity);

		// Copy over each element
		for(int i = 0; i < other._count; ++i)
			new(_data + i) T(other[i]);

		_count = other._count;
	}
	void operator=(Array_Base&& other)
	{
		empty();
		if (_data) TAllocator::free(_data);

		_data = other._data;
		_count = other._count;
		_capacity = other._capacity;

		other._data = nullptr;
		other._count = 0;
		other._capacity = 0;
	}

	void reserve(u32 num)
	{
		ensure_capacity(num);
	}

	void add(const T& item)
	{
		ensure_capacity(_count + 1);

		new(_data + _count) T(item);
		_count++;
	}
	template<typename... TArgs>
	void emplace(TArgs... args)
	{
		ensure_capacity(_count + 1);

		new(_data + _count) T(args...);
		_count++;
	}
	T& add_default()
	{
		ensure_capacity(_count + 1);
		new(_data + _count) T();
		_count++;

		return _data[_count - 1];
	}
	T& add_uninitialized()
	{
		ensure_capacity(_count + 1);
		_count++;

		return _data[_count - 1];
	}

	bool remove(const T& item)
	{
		u32 index = find(item);
		if (index == INDEX_NONE)
			return false;

		remove_at(index);
		return true;
	}

	bool remove_swap(const T& item)
	{
		u32 index = find(item);
		if (index == INDEX_NONE)
			return false;

		remove_at_swap(index);
		return true;
	}

	void remove_at(u32 index)
	{
		_data[index].~T();
		_count--;

		// This was the last element, nothing to move!
		if (index == _count)
			return;

		// Move everything after index back one spot
		memmove(_data + index, _data + index + 1, (_count - index) * sizeof(T));
	}

	void remove_at_swap(u32 index)
	{
		_data[index].~T();
		_count--;

		// This was the last element, nothing to swap!
		if (index == _count)
			return;

		memcpy(_data + index, _data + _count, sizeof(T));
	}

	Array_Base sub_array(u32 sub_offset, u32 sub_count)
	{
		// You can't grab more items than I have!!
		sub_count = Math::min(sub_count, _count - sub_offset);

		// Either count is zero, or the offset is not valid.
		if (sub_count == 0 || sub_offset >= _count)
			return Array_Base();

		auto sub = Array_Base();
		sub.reserve(sub_count);

		// Copy over elements!
		for(u32 i = 0; i < sub_count; ++i)
			sub.add(_data[i + sub_offset]);

		return sub;
	}

	template<typename... TArgs>
	void set_count(u32 new_count, TArgs... args)
	{
		// Setting count to a lower number, remove elements
		if (new_count < _count)
		{
			for(u32 i = new_count; i < _count; ++i)
				_data[i].~T();
		}
		// Setting count to a bigger number, construct elements
		if (new_count > _count)
		{
			ensure_capacity(new_count);
			for(u32 i = _count; i < new_count; ++i)
				new(_data + i) T(args...);
		}

		_count = new_count;
	}

	// Stack-like functions
	T& top()
	{
		if (_count == 0)
			fatal("Array_Base::top called in an empty array");

		return _data[_count - 1];
	}
	const T& top() const
	{
		if (_count == 0)
			fatal("Array_Base::top called in an empty array");

		return _data[_count - 1];
	}

	void pop()
	{
		if (_count == 0)
			fatal("Array_Base::pop called in an empty array");

		_count--;
		_data[_count].~T();
	}

	T&& pop_copy()
	{
		if (_count == 0)
			fatal("Array_Base::pop_copy called in an empty array");

		T copy = _data[_count - 1];
		pop();

		return move(copy);
	}

	u32 find(const T& other) const
	{
		for(u32 i = 0; i < _count; ++i)
		{
			if (_data[i] == other)
				return i;
		}

		return INDEX_NONE;
	}
	bool contains(const T& other) const
	{
		return find(other) != INDEX_NONE;
	}
	template<typename TFunctor>
	u32 find_predicate(TFunctor func) const
	{
		for(u32 i = 0; i < _count; ++i)
		{
			if (func(_data[i]))
				return i;
		}

		return INDEX_NONE;
	}

	void empty()
	{
		// Destruct all items
		for(u32 i = 0; i < _count; ++i)
			_data[i].~T();

		_count = 0;
	}

	void reset()
	{
		empty();
		if (_data) TAllocator::free(_data);
		_data = nullptr;
		_count = 0;
		_capacity = 0;
	}

	Iterator begin() { return Iterator(*this, 0); }
	Iterator end() { return Iterator(*this, _count); }
	Const_Iterator begin() const { return Const_Iterator(*this, 0); }
	Const_Iterator end() const { return Const_Iterator(*this, _count); }

private:
	void ensure_capacity(u32 new_capacity)
	{
		if (_capacity >= new_capacity)
			return;

		_capacity = Math::ceil_po2(new_capacity);
		T* new_data = TAllocator::malloc<T>(sizeof(T) * _capacity);

		if (_data)
		{
			memcpy(new_data, _data, sizeof(T) * _count);
			TAllocator::free(_data);
		}

		_data = new_data;
	}

	T* _data = nullptr;
	u32 _capacity = 0;
	u32 _count = 0;
};

template<typename T>
using Array = Array_Base<T, Heap_Allocator>;

template<typename T>
using TArray = Array_Base<T, Temp_Allocator>;