#include "file_stream.h"
#include "core/io.h"
#include <stdlib.h>
#include <stdarg.h>

bool File_Stream::open(const String& path)
{
	offset = 0;
	buffer = load_whole_file(path.data());

	// Failed to read
	if (buffer.is_null())
		return false;

	return true;
}

void File_Stream::close()
{
	buffer = String();
	offset = 0;
}

String File_Stream::read(u32 bytes)
{
	// Clamp bytes to available remaining
	if (bytes > remaining())
		bytes = remaining();

	String result = String(buffer.data() + offset, bytes);
	offset += bytes;

	return result;
}

u32 File_Stream::read(u32 bytes, void* ptr)
{
	// Clamp bytes to available remaining
	if (bytes > remaining())
		bytes = remaining();

	memcpy(ptr, buffer.data() + offset, bytes);
	offset += bytes;

	return bytes;
}

bool File_Stream::read_i32(i32* out_i32)
{
	const char* ptr = buffer.data() + offset;
	char* end = nullptr;
	*out_i32 = strtol(ptr, &end, 10);

	// Update offset
	offset = end - buffer.data();
	return true;
}

bool File_Stream::read_float(float* out_flt)
{
	const char* ptr = buffer.data() + offset;
	char* end = nullptr;
	*out_flt = strtof(ptr, &end);

	// Update offset
	offset = end - buffer.data();
	return true;
}

bool File_Stream::skip_to(char byte)
{
	while(!eof() && buffer[offset] != byte)
		offset++;

	return !eof();
}

bool File_Stream::skip_to_next_line()
{
	const char* ptr = buffer.data() + offset;

	// Skip to newline
	while(*ptr && (*ptr != '\n' && *ptr != '\r'))
		ptr++;

	// Skip past newline
	while(*ptr && (*ptr == '\n' || *ptr == '\r'))
		ptr++;

	// Update offset
	offset = ptr - buffer.data();
	return *ptr;
}

bool File_Stream::skip_substr(const char* substr, i32 length)
{
	if (length < 0)
		length = (i32)strlen(substr);

	if (strncmp(substr, current_ptr(), length) == 0)
	{
		offset += length;
		return true;
	}

	return false;
}

u32 File_Stream::scan(const char* format, ...)
{
	va_list vl;
	va_start(vl, format);

	u32 num_read = 0;
	const char* ptr = current_ptr();
	while(*format && *ptr)
	{
		// Specifier
		if (*format == '%')
		{
			// Divirging from std standard here, but I don't want whitespace to be
			//	skipped when parsing numbers :|
			bool is_numeric = (
				*ptr == '-' ||
				*ptr == '+' ||
				(*ptr >= '0' && *ptr <= '9')
			);

			// Okay no, its not a numeric, get out
			if (!is_numeric)
				break;

			format++;
			void* val = va_arg(vl, void*);
			char* end;

			switch(*format)
			{
				case 'd':
				case 'i':
					*(int*)val = strtol(ptr, &end, 10);
					break;

				case 'f':
					*(float*)val = strtof(ptr, &end);
					break;
			}

			// Failed to parse number, damnit
			if (ptr == end)
				break;

			ptr = end;
			num_read++;
			format++;
		}
		// otherwise, normal character matching
		else
		{
			// Mismatch! Get out!
			if (*format != *ptr)
				break;

			format++;
			ptr++;
		}
	}

	// Update offset
	offset = ptr - buffer.data(); 

	va_end(vl);
	return num_read;
}