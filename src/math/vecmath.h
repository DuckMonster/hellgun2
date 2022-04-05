#pragma once
#include "vec.h"
extern "C"
{
	float sqrtf(float);
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

inline Vec3 normalize(const Vec3& v)
{
	return v / length(v);
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