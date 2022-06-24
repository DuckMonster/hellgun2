#pragma once
#include <string.h>
#include <stdio.h>
#include "core/alloc/heap_allocator.h"
#include "core/alloc/temp_allocator.h"

template<typename TAllocator>
struct String_Base
{
	template<typename>
	friend struct String_Base;

	static String_Base printf(const char* format, ...);

	struct Iterator
	{
		Iterator(const String_Base<TAllocator>& str, int index) : str(str), index(index) {}

		Iterator& operator++() { index++; return *this; }
		Iterator& operator--() { index--; return *this; }
		char operator*() { return str[index]; }

		bool operator==(const Iterator& other) { return index == other.index; }
		bool operator!=(const Iterator& other) { return index != other.index; }

		const String_Base<TAllocator>& str;
		int index;
	};

	String_Base() : _data(nullptr), _length(0), _capacity(0) {}
	String_Base(const char* c_str)
	{
		u32 len = (u32)strlen(c_str);
		_capacity = len + 1;
		_length = len;

		_data = (char*)TAllocator::malloc(len + 1);
		memcpy(_data, c_str, len + 1);
	}
	String_Base(const char* c_str, u32 length)
	{
		_capacity = length + 1;
		_length = length;

		_data = (char*)TAllocator::malloc(length + 1);
		memcpy(_data, c_str, length);
		_data[length] = 0;
	}
	String_Base(const String_Base& other)
	{
		_capacity = other._capacity;
		_length = other._length;

		_data = (char*)TAllocator::malloc(other._capacity);
		memcpy(_data, other._data, other._length + 1);
	}
	template<typename TOtherAllocator>
	String_Base(const String_Base<TOtherAllocator>& other)
	{
		_capacity = other._capacity;
		_length = other._length;

		_data = (char*)TAllocator::malloc(other._capacity);
		memcpy(_data, other._data, other._length + 1);
	}
	String_Base(String_Base&& other)
	{
		_data = other._data;
		_length = other._length;
		_capacity = other._capacity;

		other._data = nullptr;
		other._length = 0;
		other._capacity = 0;
	}
	~String_Base()
	{
		if (_data)
			TAllocator::free(_data);
	}

	bool is_empty() const { return _length == 0; }
	bool is_null() const { return _data == nullptr; }

	u32 length() const { return _length; }
	u32 capacity() const { return _capacity; }
	char* data() { return _data; }
	const char* data() const { return _data; }

	char& operator[](u32 index) { return _data[index]; }
	char operator[](u32 index) const { return _data[index]; }

	void operator=(const String_Base& other)
	{
		// We already have the capacity to store this
		if (_capacity > other._capacity)
		{
			memcpy(_data, other._data, other._length + 1);
			_length = other._length;

			return;
		}

		_capacity = other._capacity;
		_length = other._length;

		_data = (char*)TAllocator::malloc(other._capacity);
		memcpy(_data, other._data, other._length + 1);
	}
	template<typename TOtherAllocator>
	void operator=(const String_Base<TOtherAllocator>& other)
	{
		// We already have the capacity to store this
		if (_capacity > other._capacity)
		{
			memcpy(_data, other._data, other._length + 1);
			_length = other._length;

			return;
		}

		_capacity = other._capacity;
		_length = other._length;

		_data = (char*)TAllocator::malloc(other._capacity);
		memcpy(_data, other._data, other._length + 1);
	}
	void operator=(String_Base&& other)
	{
		if (_data) TAllocator::free(_data);

		_data = other._data;
		_length = other._length;
		_capacity = other._capacity;

		other._data = nullptr;
		other._length = 0;
		other._capacity = 0;
	}

	// Size does NOT count null terminator
	void reserve(int new_size)
	{
		ensure_capacity(new_size + 1);
	}
	void resize(int new_size)
	{
		ensure_capacity(new_size + 1);
		_length = new_size;
	}

	// Find functions etc.
	bool ends_with(const String_Base& substr) const
	{
		// Well the substr is longer than ours so...
		if (_length < substr._length)
			return false;

		return strncmp(_data + (_length - substr._length), substr._data, substr._length) == 0;
	}

	// Various utility functions
	String_Base substr(u32 offset, u32 count) const
	{
		if (offset > _length)
			offset = _length;
		if (offset + count > _length)
			count = _length - offset;

		return String_Base(_data + offset, count);
	}
	String_Base right_chop(u32 count) const
	{
		return substr(0, _length - count);
	}

	// Addition/etc
	String_Base operator+(const String_Base& other) const
	{
		String_Base result;
		result.resize(length() + other.length());
		memcpy(result.data(), data(), length());
		memcpy(result.data() + length(), other.data(), other.length());
		result[result.length()] = 0;

		return result;
	}
	String_Base& operator+=(const String_Base& other)
	{
		reserve(_length + other._length);
		memcpy(_data + _length, other._data, other._length + 1);
		_length += other._length;

		return *this;
	}
	String_Base operator+(const char* c_str) const
	{
		String_Base result;

		u32 c_str_len = (u32)strlen(c_str);
		result.resize(length() + c_str_len);
		memcpy(result.data(), data(), length());
		memcpy(result.data() + length(), c_str, c_str_len);
		result[result.length()] = 0;

		return result;
	}

	template<typename TOtherAllocator>
	bool operator==(const String_Base<TOtherAllocator>& other) const
	{
		if (other.length() != length()) 
			return false;

		return memcmp(data(), other.data(), length()) == 0;
	}
	bool operator!=(const String_Base& other) const
	{
		if (other.length() != length()) 
			return true;

		return memcmp(data(), other.data(), length()) != 0;
	}

	// Iterator stuff
	const Iterator begin() const { return Iterator(*this, 0); }
	const Iterator end() const { return Iterator(*this, _length); }

private:
	void ensure_capacity(u32 new_capacity)
	{
		if (_capacity >= new_capacity)
			return;

		char* new_data = (char*)TAllocator::malloc(new_capacity);
		if (_data)
		{
			memcpy(new_data, _data, _length + 1);
			TAllocator::free(_data);
		}

		_data = new_data;
		_capacity = new_capacity;
	}

	char* _data = nullptr;
	u32 _length = 0;
	u32 _capacity = 0;
};

using String = String_Base<Heap_Allocator>;
using TString = String_Base<Temp_Allocator>;