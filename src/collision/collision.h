#pragma once
#include "aabb.h"

struct Hit_Result
{
	// HELPER STATICS
	static Hit_Result make_no_hit(const Vec3& location)
	{
		Hit_Result result;
		result.location = location;

		return result;
	}

	static Hit_Result make_hit(const Vec3& location, const Vec3& normal, float time)
	{
		Hit_Result result;
		result.has_hit = true;
		result.location = location;
		result.normal = normal;
		result.time = time;

		return result;
	}

	static Hit_Result make_penetrating(const Vec3& location, const Vec3& depen_vector)
	{
		Hit_Result result;
		result.has_hit = true;
		result.is_penetrating = true;
		result.location = location;

		result.penetration_depth = length(depen_vector);
		result.normal = depen_vector / result.penetration_depth;

		return result;
	}
	// ----

	bool has_hit = false;
	bool is_penetrating = false;

	Vec3 location = Vec3::zero;
	Vec3 normal = Vec3::zero;

	float time = 0.f;
	float penetration_depth = 0.f;
};

Hit_Result intersect_aabb(const AABB& src, const AABB& tar);
Hit_Result sweep_aabb(const AABB& src, const Vec3& delta, const AABB& tar);