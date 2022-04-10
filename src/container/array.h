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
		Iterator(Array<T>& arr, int index) : arr(arr), index(index) {}

		Iterator& operator++() { index++; return *this; }
		Iterator& operator--() { index--; return *this; }
		T& operator*() { return arr[index]; }

		bool operator==(const Iterator& other) { return index == other.index; }
		bool operator!=(const Iterator& other) { return index != other.index; }

		Array<T>& arr;
		int index = 0;
	};

	struct Const_Iterator
	{
		Const_Iterator(const Array<T>& arr, int index) : arr(arr), index(index) {}

		Const_Iterator& operator++() { index++; return *this; }
		Const_Iterator& operator--() { index--; return *this; }
		const T& operator*() { return arr[index]; }

		bool operator==(const Const_Iterator& other) { return index == other.index; }
		bool operator!=(const Const_Iterator& other) { return index != other.index; }

		const Array<T>& arr;
		int index = 0;
	};

	Array() {}
	Array(const Array& other)
	{
		_data = (T*)malloc(other.data_size());
		_count = other._count;
		_capacity = other._capacity;

		for(int i = 0; i < _count; ++i)
			new(_data + i) T(other[i]);
	}
	Array(Array&& other)
	{
		_data = other._data;
		_count = other._count;
		_capacity = other._capacity;

		other._data = nullptr;
		other._count = 0;
		other._capacity = 0;
	}
	~Array()
	{
		for(int i = 0; i < _count; ++i)
			_data[i].~T();

		if (_data)
			free(_data);
	}

	u32 capacity() const { return _capacity; }
	u32 count() const { return _count; }
	T* data() { return _data; }
	const T* data() const { return _data; }

	u32 data_size() const { return sizeof(T) * _count; }

	T& operator[](u32 index) { return _data[index]; }
	const T& operator[](u32 index) const { return _data[index]; }

	void operator=(const Array& other)
	{
		empty();
		ensure_capacity(other._capacity);

		// Copy over each element
		for(int i = 0; i < other._count; ++i)
			new(_data + i) T(other[i]);

		_count = other._count;
	}
	void operator=(Array&& other)
	{
		empty();
		if (_data) free(_data);

		_data = other._data;
		_count = other._count;
		_capacity = other._capacity;

		other._data = nullptr;
		other._count = 0;
		other._capacity = 0;
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

	u32 find(const T& other)
	{
		for(u32 i = 0; i < _count; ++i)
		{
			if (_data[i] == other)
				return i;
		}

		return INDEX_NONE;
	}
	template<typename TFunctor>
	u32 find_predicate(TFunctor func)
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
		for(int i = 0; i < _count; ++i)
			_data[i].~T();

		_count = 0;
	}

	void reset()
	{
		empty();
		if (_data) free(_data);
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
		T* new_data = (T*)malloc(sizeof(T) * _capacity);

		if (_data)
		{
			memcpy(new_data, _data, sizeof(T) * _count);
			free(_data);
		}

		_data = new_data;
		printf("array grow: %u\n", _capacity);
	}

	T* _data = nullptr;
	u32 _capacity = 0;
	u32 _count = 0;
};