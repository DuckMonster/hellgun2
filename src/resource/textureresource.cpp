#include "textureresource.h"
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

	if (tga.data == nullptr)
		return;

	texture.load_data(tga.data, tga.width, tga.height, tga.channels);
}

void Texture_Resource::free()
{
	texture.free();
}