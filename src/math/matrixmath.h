#include "matrix.h"

inline Mat4 mat_ortho(float left, float right, float bottom, float top, float near = -1.f, float far = 1.f)
{
	return Mat4(
		2.f / (right - left), 0.f, 0.f, 0.f,
		0.f, 2.f / (top - bottom), 0.f, 0.f,
		0.f, 0.f, -2.f  / (far - near), 0.f,
		-((right + left) / (right - left)), -((top + bottom) / (top - bottom)), -((far + near) / (far - near)), 1.f
	);
}

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

inline Mat4 inverse(const Mat4& m)
{
	float* src = (float*)&m;
	// Copied straight from Unreal! Read up on this someday.
	const float s0  = (float)(src[ 0]); const float s1  = (float)(src[ 1]); const float s2  = (float)(src[ 2]); const float s3  = (float)(src[ 3]);
	const float s4  = (float)(src[ 4]); const float s5  = (float)(src[ 5]); const float s6  = (float)(src[ 6]); const float s7  = (float)(src[ 7]);
	const float s8  = (float)(src[ 8]); const float s9  = (float)(src[ 9]); const float s10 = (float)(src[10]); const float s11 = (float)(src[11]);
	const float s12 = (float)(src[12]); const float s13 = (float)(src[13]); const float s14 = (float)(src[14]); const float s15 = (float)(src[15]);

	float inv[16];
	inv[0]  =  s5 * s10 * s15 - s5 * s11 * s14 - s9 * s6 * s15 + s9 * s7 * s14 + s13 * s6 * s11 - s13 * s7 * s10;
	inv[1]  = -s1 * s10 * s15 + s1 * s11 * s14 + s9 * s2 * s15 - s9 * s3 * s14 - s13 * s2 * s11 + s13 * s3 * s10;
	inv[2]  =  s1 * s6  * s15 - s1 * s7  * s14 - s5 * s2 * s15 + s5 * s3 * s14 + s13 * s2 * s7  - s13 * s3 * s6;
	inv[3]  = -s1 * s6  * s11 + s1 * s7  * s10 + s5 * s2 * s11 - s5 * s3 * s10 - s9  * s2 * s7  + s9  * s3 * s6;
	inv[4]  = -s4 * s10 * s15 + s4 * s11 * s14 + s8 * s6 * s15 - s8 * s7 * s14 - s12 * s6 * s11 + s12 * s7 * s10;
	inv[5]  =  s0 * s10 * s15 - s0 * s11 * s14 - s8 * s2 * s15 + s8 * s3 * s14 + s12 * s2 * s11 - s12 * s3 * s10;
	inv[6]  = -s0 * s6  * s15 + s0 * s7  * s14 + s4 * s2 * s15 - s4 * s3 * s14 - s12 * s2 * s7  + s12 * s3 * s6;
	inv[7]  =  s0 * s6  * s11 - s0 * s7  * s10 - s4 * s2 * s11 + s4 * s3 * s10 + s8  * s2 * s7  - s8  * s3 * s6;
	inv[8]  =  s4 * s9  * s15 - s4 * s11 * s13 - s8 * s5 * s15 + s8 * s7 * s13 + s12 * s5 * s11 - s12 * s7 * s9;
	inv[9]  = -s0 * s9  * s15 + s0 * s11 * s13 + s8 * s1 * s15 - s8 * s3 * s13 - s12 * s1 * s11 + s12 * s3 * s9;
	inv[10] =  s0 * s5  * s15 - s0 * s7  * s13 - s4 * s1 * s15 + s4 * s3 * s13 + s12 * s1 * s7  - s12 * s3 * s5;
	inv[11] = -s0 * s5  * s11 + s0 * s7  * s9  + s4 * s1 * s11 - s4 * s3 * s9  - s8  * s1 * s7  + s8  * s3 * s5;
	inv[12] = -s4 * s9  * s14 + s4 * s10 * s13 + s8 * s5 * s14 - s8 * s6 * s13 - s12 * s5 * s10 + s12 * s6 * s9;
	inv[13] =  s0 * s9  * s14 - s0 * s10 * s13 - s8 * s1 * s14 + s8 * s2 * s13 + s12 * s1 * s10 - s12 * s2 * s9;
	inv[14] = -s0 * s5  * s14 + s0 * s6  * s13 + s4 * s1 * s14 - s4 * s2 * s13 - s12 * s1 * s6  + s12 * s2 * s5;
	inv[15] =  s0 * s5  * s10 - s0 * s6  * s9  - s4 * s1 * s10 + s4 * s2 * s9  + s8  * s1 * s6  - s8  * s2 * s5;

	float det = s0 * inv[0] + s1 * inv[4] + s2 * inv[8] + s3 * inv[12];
	if( det != 0.0 )
	{
		det = 1.0 / det;
	}
	for( int i = 0; i < 16; i++ )
	{
		inv[i] = inv[i] * det;
	}

	return *(Mat4*)inv;
}