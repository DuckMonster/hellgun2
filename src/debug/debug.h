#pragma once
#include "math/matrix.h"
#include "container/array.h"
#include "gfx/mesh.h"
#include "gfx/render_info.h"

struct Hit_Result;

struct Debug
{
private:
	struct Draw_Info
	{
		Mesh* mesh;
		Mat4 transform;
		Color color;
		float thickness;
	};
	Array<Draw_Info> draw_list;

	struct Text_Info
	{
		String string;

		Vec2 position;
		Vec2 alignment;

		Color foreground;
		Color background;
	};
	Array<Text_Info> text_list;

	struct Print_Info
	{
		String string;
		float print_time;
		float duration;
	};
	Array<Print_Info> print_list;

public:
	void init();

	void line(const Vec3& from, const Vec3& to, const Color& color = Color::red, float thickness = 2.f);
	void point(const Vec3& position, const Color& color = Color::red, float size = 10.f);
	void vector(const Vec3& origin, const Vec3& vec, const Color& color = Color::red, float thickness = 2.f, float point_size = 10.f);

	void box(const Mat4& transform, const Color& color = Color::red, float thickness = 2.f);
	void box(const Vec3& position, const Vec3& scale = Vec3(1.f), const Quat& rotation = Quat::identity, const Color& color = Color::red, float thickness = 2.f);

	void rect(const Vec3& center, Vec3 normal, Vec3 up, const Vec2& size, const Color& color = Color::red, float thickness = 2.f);

	void sphere(const Vec3& position, float radius, const Color& color = Color::red, float thickness = 2.f);

	void capsule(const Vec3& a, const Vec3& b, float radius, const Color& color = Color::red, float thickness = 2.f);

	void hit_result(const Hit_Result& hit);

	void text(const String& str, const Vec2& position, const Color& foreground = Color::white, const Color& background = Color::black, const Vec2& alignment = Vec2::zero);
	void print(const String& str, float duration = 0.f);

	void render(const Render_Info& info);
};
extern Debug* debug;