#pragma once
#include <string.h>
#include <stdio.h>

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

		_data = (char*)malloc(len + 1);
		memcpy(_data, c_str, len + 1);
	}
	String(const String& other)
	{
		_capacity = other._capacity;
		_length = other._length;

		_data = (char*)malloc(other._capacity);
		memcpy(_data, other._data, other._length + 1);
	}
	String(String&& other)
	{
		_data = other._data;
		_length = other._length;
		_capacity = other._capacity;

		other._data = nullptr;
		other._length = 0;
		other._capacity = 0;
	}
	~String()
	{
		if (_data)
			free(_data);
	}

	u32 length() const { return _length; }
	u32 capacity() const { return _capacity; }
	char* data() { return _data; }
	const char* data() const { return _data; }

	char& operator[](u32 index) { return _data[index]; }
	char operator[](u32 index) const { return _data[index]; }

	void operator=(const String& other)
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

		_data = (char*)malloc(other._capacity);
		memcpy(_data, other._data, other._length + 1);
	}
	void operator=(String&& other)
	{
		if (_data) free(_data);

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

	// Addition/etc
	String operator+(const String& other) const
	{
		String result;
		result.resize(length() + other.length());
		memcpy(result.data(), data(), length());
		memcpy(result.data() + length(), other.data(), other.length());
		result[result.length()] = 0;

		return result;
	}
	String& operator+=(const String& other)
	{
		reserve(_length + other._length);
		memcpy(_data + _length, other._data, other._length + 1);
		_length += other._length;

		return *this;
	}
	String operator+(const char* c_str) const
	{
		String result;

		u32 c_str_len = strlen(c_str);
		result.resize(length() + c_str_len);
		memcpy(result.data(), data(), length());
		memcpy(result.data() + length(), c_str, c_str_len);
		result[result.length()] = 0;

		return result;
	}

	bool operator==(const String& other) const
	{
		if (other.length() != length()) 
			return false;

		return memcmp(data(), other.data(), length()) == 0;
	}
	bool operator!=(const String& other) const
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