#include "quat.h"

const Quat Quat::identity(0.f, 0.f, 0.f, 1.f);

Quat Quat::from_x(const Vec3& x)
{
	Mat4 mat = mat_orient_x(x);
	return Quat(mat);
}

Quat::Quat(const Mat4& m)
{
	float tr = m.m00 + m.m11 + m.m22;
	if (tr > 0.f)
	{
		float S = Math::sqrt(tr + 1.f) * 2.f;
		w = S * 0.25f;
		x = (m.m12 - m.m21) / S;
		y = (m.m20 - m.m02) / S;
		z = (m.m01 - m.m10) / S;
	}
	else if (m.m00 > m.m11 && m.m00 > m.m22)
	{
		float S = Math::sqrt(1.f + m.m00 - m.m11 - m.m22) * 2.f;
		w = (m.m12 - m.m21) / S;
		x = S * 0.25f;
		y = (m.m10 + m.m01) / S;
		z = (m.m20 + m.m02) / S;
	}
	else if (m.m11 > m.m22)
	{
		float S = Math::sqrt(1.f + m.m11 - m.m00 - m.m22) * 2.f;
		w = (m.m20 - m.m02) / S;
		x = (m.m10 + m.m01) / S;
		y = S * 0.25f;
		z = (m.m21 + m.m12) / S;
	}
	else
	{
		float S = Math::sqrt(1.f + m.m22 - m.m00 - m.m11) * 2.f;
		w = (m.m01 - m.m10) / S;
		x = (m.m20 + m.m02) / S;
		y = (m.m21 + m.m12) / S;
		z = S * 0.25f;
	}
}

Mat4 Quat::matrix() const
{
	// Copied from Unreal! Read up on this!
	Mat4 m;
	const float x2 = x + x;			const float y2 = y + y;			const float z2 = z + z;
	const float xx = x * x2;		const float xy = x * y2;		const float xz = x * z2;
	const float yy = y * y2;		const float yz = y * z2;		const float zz = z * z2;
	const float wx = w * x2;		const float wy = w * y2;		const float wz = w * z2;

	m.m00 = 1.0f - (yy + zz);	m.m10 = xy - wz;				m.m20 = xz + wy;			m.m30 = 0.f;
	m.m01 = xy + wz;			m.m11 = 1.0f - (xx + zz);		m.m21 = yz - wx;			m.m31 = 0.f;
	m.m02 = xz - wy;			m.m12 = yz + wx;				m.m22 = 1.0f - (xx + yy);	m.m32 = 0.f;
	m.m03 = 0.0f;				m.m13 = 0.0f;					m.m23 = 0.0f;				m.m33 = 1.0f;

	return m;
}