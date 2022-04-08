#include "gridfont.h"
#include "import/tga.h"

static const float QUAD[] = {
	-1.f, -1.f,		0.f, 0.f,
	1.f, -1.f,		1.f, 0.f,
	1.f, 1.f,		1.f, 1.f,

	-1.f, -1.f,		0.f, 0.f,
	1.f, 1.f,		1.f, 1.f,
	-1.f, 1.f,		0.f, 1.f,
};

void Grid_Font::load_file(const char* path, u32 glyph_w, u32 glyph_h)
{
	mesh.init();
	mesh.add_buffer(0);
	mesh.bind_attribute(0, 0, 2, 4 * sizeof(float), 0);
	mesh.bind_attribute(0, 1, 2, 4 * sizeof(float), 2 * sizeof(float));

	mesh.buffer_data(0, sizeof(QUAD), QUAD);
	mesh.draw_num = 6;

	mat.load_file("res/shader/gridfont.vert", "res/shader/gridfont.frag");

	Tga_File tga;
	tga.load("res/ui/font.tga");
	tga.flip_vertical();

	this->glyph_w = glyph_w;
	this->glyph_h = glyph_h;

	glyph_u = (float)glyph_w / tga.width;
	glyph_v = (float)glyph_h / tga.height;
	stride_u = (float)(glyph_w + 1) / tga.width;
	stride_v = (float)(glyph_h + 1) / tga.height;

	row_count = tga.width / (glyph_w + 1);

	texture.init();
	texture.load_data(tga.data, tga.width, tga.height, tga.channels);
}

void Grid_Font::render_text(const String& str, Vec2 position, const Grid_Font_Info& font_info, const Render_Info& render_info)
{
	glyphs.empty();

	Vec2 string_size = Vec2(str.length() * glyph_w);
	position -= string_size * font_info.alignment;

	for(char c : str)
	{
		push_glyph(c, position);
		position.x += glyph_w;
	}

	mesh.buffer_data(0, glyphs.count() * sizeof(Glyph), glyphs.data());
	mesh.draw_num = glyphs.count() * 6;

	mat.use();
	mat.set("u_ViewProjection", render_info.ui_canvas);
	mat.set("u_Foreground", font_info.foreground);
	mat.set("u_Background", font_info.background);

	texture.bind();
	mesh.draw();
}

void Grid_Font::push_glyph(char chr, const Vec2& position)
{
	Glyph glyph;

	u32 x = chr % row_count;
	u32 y = chr / row_count;

	Vec2 uv_base = Vec2(stride_u * x, 1.f - stride_v * y);
	Vec2 u = Vec2(glyph_u, 0.f);
	Vec2 v = Vec2(0.f, glyph_v);

	Vec2 w = Vec2(glyph_w, 0.f);
	Vec2 h = Vec2(0.f, glyph_h);

	glyph.vertices[0] = { position, uv_base };
	glyph.vertices[1] = { position + w, uv_base + u };
	glyph.vertices[2] = { position + w + h, uv_base + u - v };

	glyph.vertices[3] = { position, uv_base };
	glyph.vertices[4] = { position + w + h, uv_base + u - v };
	glyph.vertices[5] = { position + h, uv_base - v };

	glyphs.add(glyph);
}