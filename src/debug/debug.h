#pragma once
#include "math/matrix.h"
#include "container/array.h"
#include "gfx/mesh.h"
#include "game/renderinfo.h"

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

public:
	void init();

	void line(const Vec3& from, const Vec3& to, const Color& color = Color::red, float thickness = 2.f);
	void point(const Vec3& position, const Color& color = Color::red, float size = 10.f);
	void vector(const Vec3& origin, const Vec3& vec, const Color& color = Color::red, float thickness = 2.f, float point_size = 10.f);

	void box(const Mat4& transform, const Color& color = Color::red, float thickness = 2.f);
	void box(const Vec3& location, const Vec3& scale = Vec3(1.f), const Quat& rotation = Quat::identity, const Color& color = Color::red, float thickness = 2.f);

	void render(const Render_Info& info);
};

extern Debug debug;