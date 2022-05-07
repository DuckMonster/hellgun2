#pragma once

struct Sphere
{
	Vec3 origin;
	float radius;

	Sphere() : origin(Vec3::zero), radius(0.f) {}
	Sphere(const Vec3& origin, float radius)
		: origin(origin), radius(radius) {}
};