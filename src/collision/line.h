#pragma once

struct Line
{
	static Line from_origin_delta(const Vec3& origin, const Vec3& delta) { return Line(origin, origin + delta); }

	Vec3 from;
	Vec3 to;

	Line(const Vec3& from, const Vec3& to) : from(from), to(to) {}
};