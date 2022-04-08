#include "ray.h"
#include "plane.h"

bool Ray::intersect(const Plane& plane, Vec3* out_location)
{
	Vec3 offset = plane.point - origin;
	float height = dot(offset, plane.normal);
	float slope = dot(direction, plane.normal);

	float distance = height / slope;
	if (distance < 0.f)
		return false;

	*out_location = get_point(distance);
	return true;
}