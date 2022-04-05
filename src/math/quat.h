#pragma once
#include "matrix.h"

struct Quat
{
	static const Quat identity;

	float x, y, z, w;
	Quat() {}
	Quat(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) {}
	Quat(const Vec3& axis, float angle)
	{
		float sin_angle = Math::sin(angle / 2.f);
		float cos_angle = Math::cos(angle / 2.f);

		x = axis.x * sin_angle;
		y = axis.y * sin_angle;
		z = axis.z * sin_angle;
		w = cos_angle;
	}

	Vec3 operator*(const Vec3& v) const
	{
		// http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
		// V' = V + 2w(Q x V) + (2Q x (Q x V))
		// refactor:
		// V' = V + w(2(Q x V)) + (Q x (2(Q x V)))
		// T = 2(Q x V);
		// V' = V + w*(T) + (Q x T)

		const Vec3 q_v(x, y, z);
		const Vec3 u = cross(q_v, v);
		const Vec3 a = cross(q_v, u) * 2.f;
		const Vec3 b = u * 2.f * w;

		return v + a + b;
	}
	Quat operator*(const Quat& q) const
	{
		// b * a will concatenate the rotations
		// result is first (a), then (b)
		// (r1, v1)(r2, v2) = (r1r2 - v1.v2, r1v2 + r2v1 + v1 q.x v2)
		return Quat(
			q.w * x + q.x * w - q.y * z + q.z * y,
			q.w * y + q.x * z + q.y * w - q.z * x,
			q.w * z - q.x * y + q.y * x + q.z * w,
			q.w * w - q.x * x - q.y * y - q.z * z
		);
	}

	Vec3 axis_x() const
	{
		return operator*(Vec3::forward);
	}
	Vec3 axis_y() const
	{
		return operator*(Vec3::up);
	}
	Vec3 axis_z() const
	{
		return operator*(Vec3::right);
	}

	Mat4 matrix() const;
};