#include "font_resource.h"
#include "import/dat.h"
#include "texture_resource.h"

void Font_Resource::init()
{
	font.init();
}

void Font_Resource::load()
{
	clear_dependencies();

	Dat_File dat;
	dat.load_file(get_absolute_path());

	if (!dat.contains_value("texture") ||
		!dat.contains_value("glyph_width") ||
		!dat.contains_value("glyph_height"))
	{
		printf("[FONT ERROR] please include 'texture', 'glyph_height', 'glyph_width' in your font file.\n");
		return;
	}

	// Load texture
	TString texture_path = dat.read_str_temp("texture");
	Texture_Resource* texture = Resource::find_or_load_resource<Texture_Resource>(texture_path);
	add_dependency(texture);

	// Load glyph info
	u32 glyph_w = dat.read_u32("glyph_width");
	u32 glyph_h = dat.read_u32("glyph_height");

	font.set_texture(&texture->texture, glyph_w, glyph_h);
	dat.free();
}

void Font_Resource::free()
{
	font.free();
}