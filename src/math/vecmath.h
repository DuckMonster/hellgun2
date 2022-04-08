#pragma once
#include "vec.h"
extern "C"
{
	float sqrtf(float);
}

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

inline Vec3 normalize(const Vec3& v)
{
	return v / length(v);
}

inline Vec3 direction_to(const Vec3& from, const Vec3& to)
{
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