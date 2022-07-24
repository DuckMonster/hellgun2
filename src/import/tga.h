#pragma once

struct Tga_File
{
	void load(const String& path);
	void flip_vertical();
	void swizzle_rgb();
	void free();

	u32 width;
	u32 height;
	u8 channels;
	char* data = nullptr;
};