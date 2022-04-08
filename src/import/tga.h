#pragma once

struct Tga_File
{
	void load(const char* path);
	void flip_vertical();
	void free();

	u32 width;
	u32 height;
	u8 channels;
	char* data = nullptr;
};