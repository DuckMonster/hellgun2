#pragma once
#include "render_info.h"
#include "mesh.h"
#include "material.h"
#include "texture.h"
#include "container/array.h"

struct Grid_Font_Info
{
	Color foreground = Color::white;
	Color background = Color::black;

	Vec2 alignment = Vec2::zero;
	float scale = 1.f;

	Grid_Font_Info() {}
	Grid_Font_Info(const Color& foreground, const Color& background, const Vec2& alignment, float scale)
		: foreground(foreground), background(background), alignment(alignment), scale(scale) {}
};

struct Grid_Font
{
	Mesh mesh;
	Material* mat;
	Texture* texture = nullptr;

	// Data for generating glyph UVs
	float glyph_w;
	float glyph_h;

	float glyph_u;
	float glyph_v;

	float stride_u;
	float stride_v;

	u32 col_count;

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

	void init();
	void free();
	void set_texture(Texture* texture, u32 glyph_w, u32 glyph_h);

	Vec2 measure_text(const TString& str, const Grid_Font_Info& font_info = Grid_Font_Info());
	void render_text(const TString& str, Vec2 position, const Grid_Font_Info& font_info, const Render_Info& render_info);

	void push_glyph(char glyph, const Vec2& position, float scale);
};