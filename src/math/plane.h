#pragma once

struct Plane
{
	Vec3 point;
	Vec3 normal;

	Plane() {}
	Plane(const Vec3& point, const Vec3& normal) : point(point), normal(normal) {}
};