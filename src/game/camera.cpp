#include "camera.h"
#include "core/context.h"

Mat4 Camera::get_view()
{
	Vec3 axis_x = rotation.axis_x();
	Vec3 axis_y = rotation.axis_y();
	Vec3 axis_z = rotation.axis_z();
	Vec3 pos = position;

	return Mat4(
		axis_x.x, axis_y.x, axis_z.x, 0.f,
		axis_x.y, axis_y.y, axis_z.y, 0.f,
		axis_x.z, axis_y.z, axis_z.z, 0.f,
		-dot(pos, axis_x), -dot(pos, axis_y), -dot(pos, axis_z), 1.f
	);
}

Mat4 Camera::get_projection()
{
	float aspect = (float)context.width / context.height;
	float tan_half_fov = Math::tan(Math::radians(fov) / 2.f);

	return Mat4(
		1.f / (aspect * tan_half_fov), 0.f, 0.f, 0.f,
		0.f, 1.f / tan_half_fov, 0.f, 0.f,
		0.f, 0.f, -((far_plane + near_plane) / (far_plane - near_plane)), -1.f,
		0.f, 0.f, -((2.f * far_plane * near_plane) / (far_plane - near_plane)), 0.f
	);
}

Mat4 Camera::get_view_projection()
{
	return get_projection() * get_view();
}