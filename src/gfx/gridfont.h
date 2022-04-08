#pragma once
#include "renderinfo.h"
#include "mesh.h"
#include "material.h"
#include "texture.h"
#include "container/array.h"

struct Grid_Font_Info
{
	Color foreground = Color::white;
	Color background = Color::black;

	Vec2 alignment = Vec2::zero;
};

struct Grid_Font
{
	Mesh mesh;
	Material mat;
	Texture texture;

	// Data for generating glyph UVs
	float glyph_w;
	float glyph_h;

	float glyph_u;
	float glyph_v;

	float stride_u;
	float stride_v;

	u32 row_count;

	struct Glyph_Vertex
	{
		Vec2 position;
		Vec2 uv;
	};
	struct Glyph
	{
		Glyph_Vertex vertices[6];
	};

	Array<Glyph> glyphs;

	void load_file(const char* path, u32 glyph_w, u32 glyph_h);
	void render_text(const String& str, Vec2 position, const Grid_Font_Info& font_info, const Render_Info& render_info);

	void push_glyph(char glyph, const Vec2& position);
};