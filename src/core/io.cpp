#include "io.h"
#include <windows.h>
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
		printf("Failed to load file '%s' (%s)\n", path, strerror(errno));
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
		printf("Failed to load file '%s' (%s)\n", path, strerror(errno));
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

Array<String> get_all_files_in_folder(const String& path)
{
	String wildcard_path = path;
	if (!path.ends_with("/"))
		wildcard_path += "/";

	wildcard_path += "*";

	Array<String> result;

	// Start looping through the files
	HANDLE find_handle;
	WIN32_FIND_DATAA find_data;

	find_handle = FindFirstFileA(wildcard_path.data(), &find_data);
	if (find_handle == INVALID_HANDLE_VALUE)
	{
		// This folder doesn't seem to exist :(
		return result;
	}

	// Loopy loop!
	do
	{
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		result.add(String(find_data.cFileName));
	} while(FindNextFileA(find_handle, &find_data) != 0);

	FindClose(find_handle);
	return result;
}