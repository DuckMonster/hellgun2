#include "binarystream.h"
#include "core/io.h"

Binary_Stream::~Binary_Stream()
{
	free();
}

bool Binary_Stream::open(const String& path)
{
	free();

	load_whole_file(path.data(), &buffer, &size);
	offset = 0;

	return buffer != nullptr;
}

void Binary_Stream::read(u32 bytes, void* ptr)
{
	if (!can_read(bytes))
	{
		error("Binary_Stream read overflow");
		return;
	}

	memcpy(ptr, current_ptr(), bytes);
	offset += bytes;
}

String Binary_Stream::read_str()
{
	u32 str_len = read<u32>();
	if (!can_read(str_len))
	{
		error("Binary_Stream read overflow");
		return String();
	}

	String result = String(buffer + offset, str_len);
	offset += str_len;

	return result;
}

void Binary_Stream::skip(u32 bytes)
{
	if (!can_read(bytes))
	{
		error("Binary_Stream skip overflow");
		return;
	}

	offset += bytes;
}

void Binary_Stream::free()
{
	if (buffer)
		::free(buffer);

	buffer = nullptr;
	offset = 0;
	size = 0;
}