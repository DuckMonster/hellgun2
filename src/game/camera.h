#pragma once
#include "math/matrix.h"

struct Camera
{
	Vec3 position = Vec3::zero;
	Quat rotation = Quat::identity;

	float fov = 90.f;

	float near = 0.1f;
	float far = 10.f;

	Vec3 forward() { return -rotation.axis_z(); }
	Vec3 up() { return rotation.axis_y(); }
	Vec3 right() { return rotation.axis_x(); }

	Mat4 get_view();
	Mat4 get_projection();
	Mat4 get_view_projection();
};