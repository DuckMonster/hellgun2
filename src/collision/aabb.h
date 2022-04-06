#pragma once

struct AABB
{
	static AABB from_center_size(const Vec3& center, const Vec3& size)
	{
		return AABB(center - size / 2.f, center + size / 2.f);
	}

	Vec3 min;
	Vec3 max;

	AABB() : min(Vec3::zero), max(Vec3::zero) {}
	AABB(const Vec3& min, const Vec3& max) : min(min), max(max) {}

	AABB move(const Vec3& delta) const { return AABB(min + delta, max + delta); }
	Vec3 center() const { return (min + max) * 0.5f; }
	Vec3 size() const { return max - min; }
};