#pragma once
#include "aabb.h"
#include "sphere.h"
#include "line.h"
#include "capsule.h"

class Collider;
struct Hit_Result
{
	// HELPER STATICS
	static Hit_Result make_no_hit(const Vec3& position)
	{
		Hit_Result result;
		result.position = position;
		result.time = 1.f;

		return result;
	}

	static Hit_Result make_hit(const Vec3& position, const Vec3& normal, float time)
	{
		Hit_Result result;
		result.has_hit = true;
		result.position = position;
		result.normal = normal;
		result.time = time;

		return result;
	}

	static Hit_Result make_penetrating(const Vec3& position, const Vec3& depen_vector)
	{
		Hit_Result result;
		result.has_hit = true;
		result.is_penetrating = true;
		result.position = position;

		result.penetration_depth = length(depen_vector);
		result.normal = depen_vector / result.penetration_depth;

		return result;
	}
	// ----

	bool has_hit = false;
	bool is_penetrating = false;

	Vec3 position = Vec3::zero;
	Vec3 normal = Vec3::zero;

	float time = 0.f;
	float penetration_depth = 0.f;

	const Collider* collider = nullptr;

	bool contains_nan() const
	{
		return Math::is_nan(time) || Math::is_nan(penetration_depth) ||
			::contains_nan(position) || ::contains_nan(normal);
	}

	Hit_Result invert(const Vec3& new_origin, const Vec3& new_delta)
	{
		Hit_Result new_hit = *this;
		new_hit.position = new_origin + new_delta * time;
		new_hit.normal = -normal;

		return new_hit;
	}
};

namespace Collision
{
	Hit_Result select_first_hit(const Hit_Result& a, const Hit_Result& b);

	// Point stuff
	Hit_Result intersect_point(const Vec3& point, const AABB& tar);

	// Line stuff
	Hit_Result line_trace(const Vec3& start, const Vec3& end, const AABB& tar);
	Hit_Result line_trace(const Vec3& start, const Vec3& end, const Sphere& tar);
	Hit_Result line_trace(const Vec3& start, const Vec3& end, const Capsule& tar);

	// AABB stuff
	Hit_Result intersect_aabb(const AABB& src, const AABB& tar);
	Hit_Result intersect_aabb(const AABB& src, const Sphere& tar);
	Hit_Result sweep_aabb(const AABB& src, const Vec3& delta, const AABB& tar);
	Hit_Result sweep_aabb(const AABB& src, const Vec3& delta, const Sphere& tar);

	// Sphere stuff
	Hit_Result intersect_sphere(const Sphere& src, const Sphere& tar);
	Hit_Result intersect_sphere(const Sphere& src, const AABB& tar);
	Hit_Result sweep_sphere(const Sphere& src, const Vec3& delta, const Sphere& tar);
	Hit_Result sweep_sphere(const Sphere& src, const Vec3& delta, const AABB& tar);
}