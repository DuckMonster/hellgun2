#pragma once

struct Capsule
{
	Vec3 a;
	Vec3 b;
	float radius;

	Capsule(const Vec3& a, const Vec3& b, float radius)
		: a(a), b(b), radius(radius) {}
};