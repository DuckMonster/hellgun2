#include "matrix.h"

inline Mat4 mat_translation(const Vec3& v)
{
	return Mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		v.x, v.y, v.z, 1.f
	);
}

inline Mat4 mat_scale(const Vec3& scale)
{
	return Mat4(
		scale.x, 0.f, 0.f, 0.f,
		0.f, scale.y, 0.f, 0.f,
		0.f, 0.f, scale.z, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

inline Mat4 mat_scale(float s)
{
	return Mat4(
		s, 0.f, 0.f, 0.f,
		0.f, s, 0.f, 0.f,
		0.f, 0.f, s, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}