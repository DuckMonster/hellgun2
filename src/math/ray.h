#pragma once

struct Plane;
struct Ray
{
	Vec3 origin;
	Vec3 direction;

	Ray() : origin(Vec3::zero), direction(Vec3::forward) {}
	Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

	Vec3 get_point(float distance) { return origin + direction * distance; }
	bool intersect(const Plane& plane, Vec3* out_location);
};