#include "io.h"
#include <stdio.h>
#include <stdlib.h>

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