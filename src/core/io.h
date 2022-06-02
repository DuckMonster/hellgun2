#pragma once
#include "container/array.h"

void load_whole_file(const char* path, char** out_data, int* out_size);
String load_whole_file(const char* path);
u64 get_file_modify_time(const char* path);

Array<String> get_all_files_in_folder(const String& path);