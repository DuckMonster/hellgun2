#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

void load_whole_file(const char* path, char** out_data, int* out_size)
{
	FILE* file = NULL;
	fopen_s(&file, path, "rb");

	if (file == NULL)
	{
		printf("Failed to load file '%s'\n", path);
		*out_data = nullptr;
		*out_size = 0;

		return;
	}

	fseek(file, 0, SEEK_END);
	*out_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	*out_data = (char*)malloc(*out_size);
	fread(*out_data, 1, *out_size, file);

	fclose(file);
}

String load_whole_file(const char* path)
{
	FILE* file = NULL;
	fopen_s(&file, path, "rb");

	if (file == NULL)
	{
		printf("Failed to load file '%s'\n", path);
		return String();
	}

	String result;

	fseek(file, 0, SEEK_END);
	u64 size = (u64)ftell(file);
	fseek(file, 0, SEEK_SET);

	// Setup string, (with null terminator)
	result.resize(size);
	result[size] = 0;

	fread(result.data(), 1, size, file);
	fclose(file);

	return result;
}

u64 get_file_modify_time(const char* path)
{
	struct _stat64 stats;
	if (_stat64(path, &stats) == 0)
		return (u64)stats.st_mtime;

	return 0;
}