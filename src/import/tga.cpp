#include "tga.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const u8 RUN_LENGTH_BIT = 0b1000;

enum class Tga_Image_Type : u8
{
	NoImageData,
	ColorMapped,
	TrueColor,
	BlackAndWhite
};

#pragma pack(push, 1)

struct Tga_Header
{
	// Length of image id field
	u8 id_length;

	// 0 if no color map, 1 if there is
	u8 color_map_type;

	// Describes the image type (colored, grayscale etc.)
	u8 image_type;
};

struct Tga_Color_Map
{
	u16 first_index;
	u16 length;
	u8 bits_per_pixel;
};

struct Tga_Image_Spec
{
	u16 x_origin;
	u16 y_origin;
	u16 width;
	u16 height;
	u8 pixel_depth;
	u8 image_descriptor;
};

#pragma pack(pop)

void Tga_File::load(const String& path)
{
	// Make sure we're freed first
	free();

	FILE* file;
	fopen_s(&file, path.data(), "rb");
	if (file == nullptr)
	{
		printf("Failed to load TGA file '%s', file doesn't exist", path.data());
		return;
	}

	// Read header
	Tga_Header header;
	Tga_Color_Map color_map;
	Tga_Image_Spec image_spec;

	fread(&header, sizeof(header), 1, file);
	fread(&color_map, sizeof(color_map), 1, file);
	fread(&image_spec, sizeof(image_spec), 1, file);

	// Read image ID
	fread(nullptr, header.id_length, 1, file);

	// Read pixel data
	width = image_spec.width;
	height = image_spec.height;
	channels = image_spec.pixel_depth / 8;

	u32 image_size = width * height * channels;
	data = (char*)malloc(image_size);
	fread(data, image_size, 1, file);

	fclose(file);
}

void Tga_File::flip_vertical()
{
	// Not loaded.
	if (!data)
		return;

	// Copy all data into a temporary buffer
	char* buffer = (char*)malloc(width * height * channels);
	memcpy(buffer, data, width * height * channels);

	// Copy over every line 
	const u32 line_size = width * channels;

	for(u32 line = 0; line < height; ++line)
	{
		char* src = buffer + (height - line - 1) * line_size;
		char* tar = data + line * line_size;

		memcpy(tar, src, line_size);
	}

	::free(buffer);
}

void Tga_File::free()
{
	if (data)
		::free(data);

	data = nullptr;
	width = 0;
	height = 0;
	channels = 0;
}