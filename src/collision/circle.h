#pragma once

struct Circle
{
	Vec2 origin;
	float radius;

	Circle() : origin(Vec2::zero), radius(0.f) {}
	Circle(const Vec2& origin, float radius)
		: origin(origin), radius(radius) {}
};