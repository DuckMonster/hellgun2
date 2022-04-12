#include "random.h"
#include <stdlib.h>

int Random::number(int max)
{
	return rand() % max;
}
int Random::range(int min, int max)
{
	if (max < min)
		swap(min, max);

	return min + (rand() % (max - min));
}

float Random::value(float max)
{
	return ((float)rand() / RAND_MAX) * max;
}

float Random::range(float min, float max)
{
	return Math::lerp(min, max, Random::value());
}

Vec3 Random::point_on_sphere(float radius)
{
	float yaw = Random::range(0.f, TAU);
	float pitch = Random::range(-PI, PI);

	return Vec3(
		Math::cos(yaw) * Math::cos(pitch),
		Math::sin(yaw) * Math::cos(pitch),
		Math::sin(pitch)
	) * radius;
}

Vec3 Random::point_on_cone(const Vec3& cone_direction, float cone_angle)
{
	float offset_angle = Math::radians(Random::range(-cone_angle, cone_angle) / 2.f);
	float roll_angle = Random::value(TAU);

	Quat offset_q = Quat(arbitrary_perpendicular(cone_direction), offset_angle);
	Quat roll_q = Quat(cone_direction, roll_angle);

	return roll_q * offset_q * cone_direction;
}

Vec3 Random::point_on_circle(const Vec3& circle_normal)
{
	float angle = Random::range(0.f, TAU);
	Vec3 perp_a = normalize(arbitrary_perpendicular(circle_normal));
	Vec3 perp_b = cross(circle_normal, perp_a);

	return perp_a * Math::sin(angle) + perp_b * Math::cos(angle);
}

Color Random::color()
{
	return Color(
		Random::value(),
		Random::value(),
		Random::value(),
		1.f
	);
}