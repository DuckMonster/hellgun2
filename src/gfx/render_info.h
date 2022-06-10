#pragma once

struct Render_Info
{
	Mat4 view;
	Mat4 projection;
	Mat4 view_projection;

	Mat4 ui_canvas;

	Vec2 world_to_canvas(const Vec3& world) const
	{
		// First apply view projection
		Vec4 ndc = view_projection * Vec4(world.x, world.y, world.z, 1.f);
		ndc /= ndc.w;

		// Then apply the inverse of canvas
		ndc = inverse(ui_canvas) * ndc;
		return Vec2(ndc.x, ndc.y);
	}
};