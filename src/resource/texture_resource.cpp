#include "texture_resource.h"
#include "import/tga.h"

void Texture_Resource::init()
{
	texture.init();
}

void Texture_Resource::load()
{
	Tga_File tga;
	tga.load(get_absolute_path());
	tga.flip_vertical();
	tga.swizzle_rgb();

	if (tga.data == nullptr)
		return;

	texture.load_data(tga.data, tga.width, tga.height, tga.channels);
}

void Texture_Resource::free()
{
	texture.free();
}