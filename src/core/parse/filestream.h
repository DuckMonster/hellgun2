#pragma once

struct File_Stream
{
	bool open(const String& path);
	void close();
	bool is_open() { return buffer.is_null(); }

	u32 size() { return buffer.length(); }
	u32 remaining() { return buffer.length() - offset; }
	u32 current_offset() { return offset; }
	const char* current_ptr() { return buffer.data() + offset; }

	bool eof() { return offset >= buffer.length(); }

	String read(u32 bytes);
	u32 read(u32 bytes, void* ptr);
	bool read_i32(i32* out_i32);
	bool read_float(float* out_flt);

	void skip(u32 bytes) { offset += bytes; }
	bool skip_to(char byte);
	bool skip_substr(const char* substr, i32 length = -1);

	// Only really valid for text files
	bool skip_to_next_line();
	u32 scan(const char* format, ...);

	template<typename FunctorT>
	bool skip_until(FunctorT predicate)
	{
		while(!eof() && !predicate(buffer[offset]))
			offset++;

		return !eof();
	}

private:
	String buffer;
	u32 offset;
};