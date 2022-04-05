#pragma once
#include <new>
#include <string.h>

#define INDEX_NONE (~0)

template<typename T>
struct Array
{
public:
	struct Iterator
	{
		Iterator(Array<T>& arr, int index) : arr(arr), index(index) {};

		Iterator& operator++() { index++; return *this; }
		Iterator& operator--() { index--; return *this; }
		T& operator*() { return arr[index]; }
		const T& operator*() const { return arr[index]; }

		bool operator==(const Iterator& other) { return index == other.index; }
		bool operator!=(const Iterator& other) { return index != other.index; }

		Array<T>& arr;
		int index = 0;
	};

	Array() {}

	u32 capacity() const { return _capacity; }
	u32 count() const { return _count; }

	T& operator[](u32 index) { return _data[index]; }
	const T& operator[](u32 index) const { return _data[index]; }

	void add(const T& item)
	{
		ensure_capacity(_count + 1);
		_data[_count++] = item;
	}
	T& add_default()
	{
		ensure_capacity(_count + 1);
		new(_data + _count) T();
		_count++;

		return _data[_count - 1];
	}
	template<typename... TArgs>
	void emplace(TArgs... args)
	{
		ensure_capacity(_count + 1);
		new(_data + _count) T(args...);

		_count++;
	}

	void remove_at(u32 index)
	{
		_count--;

		// This was the last element, nothing to move!
		if (index == _count)
			return;

		// Move everything after index back one spot
		memmove(_data + index, _data + index + 1, (_count - index) * sizeof(T));
	}

	void remove_at_swap(u32 index)
	{
		_count--;

		// This was the last element, nothing to swap!
		if (index == _count)
			return;

		memcpy(_data + index, _data + _count, sizeof(T));
	}

	u32 find(const T& other)
	{
		for(u32 i = 0; i < _count; ++i)
		{
			if (_data[i] == other)
				return i;
		}

		return INDEX_NONE;
	}

	void empty()
	{
		_count = 0;
	}

	Iterator begin() { return Iterator(*this, 0); }
	Iterator end() { return Iterator(*this, _count); }

private:
	void ensure_capacity(u32 new_capacity)
	{
		if (_capacity >= new_capacity)
			return;

		_capacity = Math::ceil_po2(new_capacity);
		T* new_data = (T*)malloc(sizeof(T) * _capacity);

		if (_data)
		{
			memcpy(new_data, _data, sizeof(T) * _count);
			free(_data);
		}

		_data = new_data;
	}

	T* _data = nullptr;
	u32 _capacity = 0;
	u32 _count = 0;
};