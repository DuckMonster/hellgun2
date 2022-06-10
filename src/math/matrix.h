#pragma once
#include "vec.h"

struct Mat4
{
	static const Mat4 identity;

	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
	float m30, m31, m32, m33;

	Mat4() {}
	Mat4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) :
		m00(m00), m01(m01), m02(m02), m03(m03),
		m10(m10), m11(m11), m12(m12), m13(m13),
		m20(m20), m21(m21), m22(m22), m23(m23),
		m30(m30), m31(m31), m32(m32), m33(m33) {}
	Mat4(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec3& v3) :
		m00(v0.x), m01(v0.y), m02(v0.z), m03(0.f),
		m10(v1.x), m11(v1.y), m12(v1.z), m13(0.f),
		m20(v2.x), m21(v2.y), m22(v2.z), m23(0.f),
		m30(v3.x), m31(v3.y), m32(v3.z), m33(1.f) {}

	Mat4 operator*(const Mat4& rhs) const
	{
		Mat4 result;

		// m00 = lhs.col(0) * rhs.row(0)
		result.m00 = m00 * rhs.m00 + m10 * rhs.m01 + m20 * rhs.m02 + m30 * rhs.m03;
		// m01 = lhs.col(1) * rhs.row(0)
		result.m01 = m01 * rhs.m00 + m11 * rhs.m01 + m21 * rhs.m02 + m31 * rhs.m03;
		// m02 = lhs.col(2) * rhs.row(0)
		result.m02 = m02 * rhs.m00 + m12 * rhs.m01 + m22 * rhs.m02 + m32 * rhs.m03;
		// m03 = lhs.col(3) * rhs.row(0)
		result.m03 = m03 * rhs.m00 + m13 * rhs.m01 + m23 * rhs.m02 + m33 * rhs.m03;

		// m10 = lhs.col(0) * rhs.row(1)
		result.m10 = m00 * rhs.m10 + m10 * rhs.m11 + m20 * rhs.m12 + m30 * rhs.m13;
		// m11 = lhs.col(1) * rhs.row(1)
		result.m11 = m01 * rhs.m10 + m11 * rhs.m11 + m21 * rhs.m12 + m31 * rhs.m13;
		// m12 = lhs.col(2) * rhs.row(1)
		result.m12 = m02 * rhs.m10 + m12 * rhs.m11 + m22 * rhs.m12 + m32 * rhs.m13;
		// m13 = lhs.col(3) * rhs.row(1)
		result.m13 = m03 * rhs.m10 + m13 * rhs.m11 + m23 * rhs.m12 + m33 * rhs.m13;

		// m20 = lhs.col(0) * rhs.row(2)
		result.m20 = m00 * rhs.m20 + m10 * rhs.m21 + m20 * rhs.m22 + m30 * rhs.m23;
		// m21 = lhs.col(1) * rhs.row(2)
		result.m21 = m01 * rhs.m20 + m11 * rhs.m21 + m21 * rhs.m22 + m31 * rhs.m23;
		// m22 = lhs.col(2) * rhs.row(2)
		result.m22 = m02 * rhs.m20 + m12 * rhs.m21 + m22 * rhs.m22 + m32 * rhs.m23;
		// m23 = lhs.col(3) * rhs.row(2)
		result.m23 = m03 * rhs.m20 + m13 * rhs.m21 + m23 * rhs.m22 + m33 * rhs.m23;

		// m30 = lhs.col(0) * rhs.row(3)
		result.m30 = m00 * rhs.m30 + m10 * rhs.m31 + m20 * rhs.m32 + m30 * rhs.m33;
		// m31 = lhs.col(1) * rhs.row(3)
		result.m31 = m01 * rhs.m30 + m11 * rhs.m31 + m21 * rhs.m32 + m31 * rhs.m33;
		// m32 = lhs.col(2) * rhs.row(3)
		result.m32 = m02 * rhs.m30 + m12 * rhs.m31 + m22 * rhs.m32 + m32 * rhs.m33;
		// m33 = lhs.col(3) * rhs.row(3)
		result.m33 = m03 * rhs.m30 + m13 * rhs.m31 + m23 * rhs.m32 + m33 * rhs.m33;

		return result;
	}

	Vec3 operator*(const Vec3& v) const
	{
		Vec3 x = Vec3(m00 * v.x, m01 * v.x, m02 * v.x);
		Vec3 y = Vec3(m10 * v.y, m11 * v.y, m12 * v.y);
		Vec3 z = Vec3(m20 * v.z, m21 * v.z, m22 * v.z);
		Vec3 w = Vec3(m30, m31, m32);

		return x + y + z + w;
	}

	Vec4 operator*(const Vec4& v) const
	{
		Vec4 x = Vec4(m00 * v.x, m01 * v.x, m02 * v.x, m03 * v.x);
		Vec4 y = Vec4(m10 * v.y, m11 * v.y, m12 * v.y, m13 * v.y);
		Vec4 z = Vec4(m20 * v.z, m21 * v.z, m22 * v.z, m23 * v.z);
		Vec4 w = Vec4(m30 * v.w, m31 * v.w, m32 * v.w, m33 * v.w);

		return x + y + z + w;
	}
};