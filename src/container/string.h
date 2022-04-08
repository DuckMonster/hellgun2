#pragma once
#include <string.h>

struct String
{
	static String printf(const char* format, ...);

	struct Iterator
	{
		Iterator(const String& str, int index) : str(str), index(index) {}

		Iterator& operator++() { index++; return *this; }
		Iterator& operator--() { index--; return *this; }
		char operator*() { return str[index]; }

		bool operator==(const Iterator& other) { return index == other.index; }
		bool operator!=(const Iterator& other) { return index != other.index; }

		const String& str;
		int index;
	};

	String() : _data(nullptr), _length(0), _capacity(0) {}
	String(const char* c_str)
	{
		u32 len = (u32)strlen(c_str);
		_capacity = len + 1;
		_length = len;

		if (_data) free(_data);

		_data = (char*)malloc(len + 1);
		memcpy(_data, c_str, len + 1);
	}
	String(const String& other)
	{
		memcpy(_data, other._data, other._length + 1);
		_capacity = other._capacity;
		_length = other._length;

		if (_data) free(_data);

		_data = (char*)malloc(other._capacity);
		memcpy(_data, other._data, other._length + 1);
	}
	~String()
	{
		//if (_data) free(_data);
	}

	u32 length() const { return _length; }
	u32 capacity() const { return _capacity; }
	char* data() { return _data; }
	const char* data() const { return _data; }

	char operator[](u32 index) const { return _data[index]; }

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

	// Iterator stuff
	const Iterator begin() const { return Iterator(*this, 0); }
	const Iterator end() const { return Iterator(*this, _length); }

private:
	void ensure_capacity(u32 new_capacity)
	{
		if (_capacity >= new_capacity)
			return;

		char* new_data = (char*)malloc(new_capacity);
		if (_data)
		{
			memcpy(new_data, _data, _length + 1);
			free(_data);
		}

		_data = new_data;
		_capacity = new_capacity;
	}

	char* _data = nullptr;
	u32 _length = 0;
	u32 _capacity = 0;
};