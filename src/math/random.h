#pragma once
#include "math/color.h"

class Random
{
public:
	static int number(int max);
	static int range(int min, int max);

	static float value(float max = 1.f);
	static float range(float min, float max);

	static Vec3 point_on_sphere(float radius);
	static Vec3 point_on_cone(const Vec3& cone_direction, float cone_angle);
	static Color color();
};