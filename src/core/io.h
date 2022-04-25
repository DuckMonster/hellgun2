#pragma once

void load_whole_file(const char* path, char** out_data, int* out_size);
String load_whole_file(const char* path);
u64 get_file_modify_time(const char* path);