#include "grid_font.h"
#include "import/tga.h"
#include "resource/resource.h"

void Grid_Font::init()
{
	mesh.init();
	mesh.add_buffer(0);
	mesh.bind_attribute(0, 0, 2, 4 * sizeof(float), 0);
	mesh.bind_attribute(0, 1, 2, 4 * sizeof(float), 2 * sizeof(float));

	mat = Resource::load_material("material/ui/gridfont.mat");
}

void Grid_Font::free()
{
	mesh.free();
}

void Grid_Font::set_texture(Texture* texture, u32 glyph_w, u32 glyph_h)
{
	this->texture = texture;
	this->glyph_w = glyph_w;
	this->glyph_h = glyph_h;

	glyph_u = (float)glyph_w / texture->width;
	glyph_v = (float)glyph_h / texture->height;
	stride_u = (float)(glyph_w + 1) / texture->width;
	stride_v = (float)(glyph_h + 1) / texture->height;

	col_count = texture->width / (glyph_w + 1);
}

Vec2 Grid_Font::measure_text(const TString& str, const Grid_Font_Info& font_info)
{
	return Vec2(str.length() * glyph_w * font_info.scale, glyph_h);
}

void Grid_Font::render_text(const TString& str, Vec2 position, const Grid_Font_Info& font_info, const Render_Info& render_info)
{
	// Not loaded...
	if (texture == nullptr)
		return;

	glyphs.empty();

	Vec2 string_size = Vec2(str.length() * glyph_w * font_info.scale);
	position -= string_size * font_info.alignment;

	for(char c : str)
	{
		push_glyph(c, position, font_info.scale);
		position.x += glyph_w * font_info.scale;
	}

	mesh.buffer_data(0, glyphs.count() * sizeof(Glyph), glyphs.data());
	mesh.draw_num = glyphs.count() * 6;

	mat->use();
	mat->set("u_ViewProjection", render_info.ui_canvas);
	mat->set("u_Foreground", font_info.foreground);
	mat->set("u_Background", font_info.background);

	texture->bind();
	mesh.draw();
}

void Grid_Font::push_glyph(char chr, const Vec2& position, float scale)
{
	Glyph glyph;

	u32 x = chr % col_count;
	u32 y = chr / col_count;

	Vec2 uv_base = Vec2(stride_u * x, 1.f - stride_v * y);
	Vec2 u = Vec2(glyph_u, 0.f);
	Vec2 v = Vec2(0.f, -glyph_v);

	Vec2 w = Vec2(glyph_w, 0.f) * scale;
	Vec2 h = Vec2(0.f, glyph_h) * scale;

	glyph.vertices[0] = { position, uv_base };
	glyph.vertices[1] = { position + h, uv_base + v };
	glyph.vertices[2] = { position + h + w, uv_base + v + u };

	glyph.vertices[3] = { position, uv_base };
	glyph.vertices[4] = { position + h + w, uv_base + v + u };
	glyph.vertices[5] = { position + w, uv_base + u };

	glyphs.add(glyph);
}