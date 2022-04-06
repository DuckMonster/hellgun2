#include "collision.h"

Hit_Result Collision::select_first_hit(const Hit_Result& a, const Hit_Result& b)
{
	if (!a.has_hit) return b;
	if (!b.has_hit) return a;
	return a.time < b.time ? a : b;
}

Hit_Result Collision::intersect_aabb(const AABB& src, const AABB& tar)
{
	struct Axis_Result
	{
		bool intersect;

		Vec3 axis;
		float depth;
	};

	auto test_axis = [&src, &tar](const Vec3& axis) -> Axis_Result
	{
		float a_min = dot(src.min, axis);
		float a_max = dot(src.max, axis);
		float b_min = dot(tar.min, axis);
		float b_max = dot(tar.max, axis);

		// Positive depth on axis
		float p_depth = b_max - a_min;

		// Negative depth on axis (must move in opposite direction to depen)
		float n_depth = a_max - b_min;

		// Depth is less than 0 in either direction
		if (p_depth <= 0.f || n_depth <= 0.f)
			return { false, axis, 0.f };

		if (p_depth < n_depth)
			return { true, axis, p_depth };
		else
			return { true, -axis, n_depth };
	};

	auto select_result = [](const Axis_Result& a, const Axis_Result& b) -> Axis_Result
	{
		if (!a.intersect)
			return a;
		if (!b.intersect)
			return b;

		return a.depth < b.depth ? a : b;
	};

	Axis_Result axis_result = test_axis(Vec3::forward);
	axis_result = select_result(axis_result, test_axis(Vec3::right));
	axis_result = select_result(axis_result, test_axis(Vec3::up));

	if (!axis_result.intersect)
		return Hit_Result::make_no_hit(src.center());

	return Hit_Result::make_penetrating(src.center(), axis_result.axis * axis_result.depth);
}

Hit_Result Collision::sweep_aabb(const AABB& src, const Vec3& delta, const AABB& tar)
{
	if (is_nearly_zero(delta))
		return intersect_aabb(src, tar);

	struct Axis_Result
	{
		Vec3 axis;
		float enter_time;
		float exit_time;
	};

	auto test_axis = [&src, &tar, &delta](const Vec3& axis) -> Axis_Result
	{
		float axis_delta = dot(delta, axis);

		float a_min = dot(src.min, axis);
		float a_max = dot(src.max, axis);
		float b_min = dot(tar.min, axis);
		float b_max = dot(tar.max, axis);

		// We dont actually move on this axis....
		// Do a regular overlap check
		if (axis_delta == 0.f)
		{
			// Intersecting, give arbitrarily large time span
			if (a_max > b_min && b_max > a_min)
				return { axis, -BIG_NUMBER, BIG_NUMBER };

			// Not intersecting, but time span as 0 somewhere far away
			else
				return { axis, BIG_NUMBER, BIG_NUMBER };
		}

		if (axis_delta > 0.f)
			return { -axis, (b_min - a_max) / axis_delta, (b_max - a_min) / axis_delta };
		else
			return { axis, (b_max - a_min) / axis_delta, (b_min - a_max) / axis_delta };
	};

	auto select_last_enter = [](const Axis_Result& a, const Axis_Result& b, const Axis_Result& c) -> Axis_Result
	{
		return a.enter_time > b.enter_time ? (a.enter_time > c.enter_time ? a : c) : (b.enter_time > c.enter_time ? b : c); 
	};

	Axis_Result x_axis = test_axis(Vec3::forward);
	Axis_Result y_axis = test_axis(Vec3::right);
	Axis_Result z_axis = test_axis(Vec3::up);

	float first_exit = Math::min(x_axis.exit_time, y_axis.exit_time, z_axis.exit_time);
	float last_enter = Math::max(x_axis.enter_time, y_axis.enter_time, z_axis.enter_time);

	// One axis exited before all of them entered, no hit
	if (first_exit < last_enter)
		return Hit_Result::make_no_hit(src.center() + delta);

	// The last enter happened at time > 1, meaning after the end of the sweep
	if (last_enter > 1.f)
		return Hit_Result::make_no_hit(src.center() + delta);

	// The last enter was before or equal to 0... that means we're penetrating
	// Use regular intersection to figure out depenetration
	if (last_enter < 0.f)
		return intersect_aabb(src, tar);

	// Get the last entry, that will be our hit
	Axis_Result last_enter_axis = select_last_enter(x_axis, y_axis, z_axis);
	return Hit_Result::make_hit(src.center() + delta * last_enter_axis.enter_time, last_enter_axis.axis, last_enter_axis.enter_time);
}