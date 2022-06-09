#pragma once

struct Binary_Stream
{
public:
	~Binary_Stream();
	bool open(const String& path);

	void read(u32 bytes, void* ptr);
	void skip(u32 bytes);

	template<typename T>
	T read()
	{
		T result;
		read(sizeof(T), &result);

		return result;
	}

	template<typename T>
	void read(T* ptr) { read(sizeof(T), ptr); }
	String read_str();

private:
	bool can_read(u32 bytes) { return (size - offset) >= bytes; }
	void free();
	char* current_ptr() { return buffer + offset; }

	char* buffer = nullptr;
	u32 offset;
	int size;
};