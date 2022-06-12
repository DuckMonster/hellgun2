#pragma once
#include "vec.h"
#include <math.h>

inline bool contains_nan(const Vec3& vec)
{
	return Math::is_nan(vec.x + vec.y + vec.z);
}

inline bool is_nearly_zero(const Vec3& vec, float check = SMALL_NUMBER)
{
	return
		Math::is_nearly_zero(vec.x, check) && 
		Math::is_nearly_zero(vec.y, check) &&
		Math::is_nearly_zero(vec.z, check);
}
inline bool is_nearly_equal(const Vec3& a, const Vec3& b, float check = SMALL_NUMBER)
{
	return
		Math::is_nearly_zero(b.x - a.x, check) && 
		Math::is_nearly_zero(b.y - a.y, check) &&
		Math::is_nearly_zero(b.z - a.z, check);
}

inline float length(const Vec2& vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}
inline float length(const Vec3& vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
inline float length_sqrd(const Vec2& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}
inline float length_sqrd(const Vec3& vec)
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

inline float distance(const Vec3& a, const Vec3& b)
{
	return length(b - a);
}
inline float distance_sqrd(const Vec3& a, const Vec3& b)
{
	return length_sqrd(b - a);
}

inline Vec2 normalize(const Vec2& v)
{
	return v / length(v);
}
inline Vec3 normalize(const Vec3& v)
{
	return v / length(v);
}

inline Vec3 direction_to(const Vec3& from, const Vec3& to)
{
	if (is_nearly_equal(from, to))
		return Vec3::zero;

	return normalize(to - from);
}

inline float dot(const Vec2& a, const Vec2& b)
{
	return a.x * b.x + a.y * b.y;
}
inline float dot(const Vec3& a, const Vec3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline Vec3 cross(const Vec3& a, const Vec3& b)
{
	return Vec3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

inline Vec3 constrain_to_direction(const Vec3& a, const Vec3& direction)
{
	return direction * dot(a, direction);
}
inline Vec3 constrain_to_plane(const Vec3& a, const Vec3& normal)
{
	return a - normal * dot(a, normal);
}

inline Vec3 arbitrary_perpendicular(const Vec3& vec)
{
	const Vec3 ARBITRARY = Vec3(125.2451f, -53.542148f, 854.25799856f);
	return normalize(cross(vec, ARBITRARY));
}

inline Vec3 component_clamp(const Vec3& v, const Vec3& min, const Vec3& max)
{
	return Vec3(
		Math::clamp(v.x, min.x, max.x),
		Math::clamp(v.y, min.y, max.y),
		Math::clamp(v.z, min.z, max.z)
	);
}

inline Vec3 component_min(const Vec3& a, const Vec3& b)
{
	return Vec3(
		Math::min(a.x, b.x),
		Math::min(a.y, b.y),
		Math::min(a.z, b.z)
	);
}
inline Vec3 component_max(const Vec3& a, const Vec3& b)
{
	return Vec3(
		Math::max(a.x, b.x),
		Math::max(a.y, b.y),
		Math::max(a.z, b.z)
	);
}

inline float vec_to_angle(const Vec2& a)
{
	return Math::atan2(a.y, a.x);
}