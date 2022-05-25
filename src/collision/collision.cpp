#include "collision.h"
#include "debug/debug.h"

Hit_Result Collision::select_first_hit(const Hit_Result& a, const Hit_Result& b)
{
	if (!a.has_hit) return b;
	if (!b.has_hit) return a;
	return a.time < b.time ? a : b;
}

Hit_Result Collision::intersect_point(const Vec3& point, const AABB& tar)
{
	Vec3 point_to_max = tar.max - point;
	Vec3 min_to_point = point - tar.min;

	float min_depth = BIG_NUMBER;
	Vec3 min_depth_normal = Vec3::zero;

	for(u32 axis = 0; axis < 3; ++axis)
	{
		float depth;
		Vec3 normal = Vec3::zero;

		if (point_to_max[axis] < min_to_point[axis])
		{
			depth = point_to_max[axis];
			normal[axis] = 1.f;
		}
		else
		{
			depth = min_to_point[axis];
			normal[axis] = -1.f;
		}

		// Hold on, this axis doesn't overlap at all! Get out!
		if (depth < 0.f)
			return Hit_Result::make_no_hit(point);

		if (depth < min_depth)
		{
			min_depth = depth;
			min_depth_normal = normal;
		}
	}

	return Hit_Result::make_penetrating(point, min_depth_normal * min_depth);
}

Hit_Result Collision::line_trace(const Vec3& start, const Vec3& end, const AABB& tar)
{
	Vec3 start_to_max = tar.max - start;
	Vec3 start_to_min = tar.min - start;
	Vec3 line_delta = end - start;

	// Sweep through each axis, calculating the enter and exit time
	// If the times overlap for all axes, thats a hit! And the last enter will be the hit point
	float last_enter_time = -BIG_NUMBER;
	float first_exit_time = BIG_NUMBER;
	Vec3 last_enter_normal;

	for(u32 axis = 0; axis < 3; ++axis)
	{
		Vec3 normal = Vec3::zero;
		normal[axis] = -1.f;

		float enter_time = 0.f;
		float exit_time = 0.f;

		// Line is parallel to this face, so check if its inside or outside
		if (Math::is_nearly_zero(line_delta[axis]))
		{
			// Start of line is fully inside the box on this axis
			if (start[axis] >= tar.min[axis] &&
				start[axis] <= tar.max[axis])
			{
				enter_time = -BIG_NUMBER;
				exit_time = BIG_NUMBER;
			}

			// Axis is outside, that means we can early out since it can't move inside
			else
			{
				return Hit_Result::make_no_hit(end);
			}
		}
		else
		{
			// Find the enter/exit time
			enter_time = start_to_min[axis] / line_delta[axis];
			exit_time = start_to_max[axis] / line_delta[axis];

			// Oop! Seems like we're going in the other direction, reverse reverse
			if (exit_time < enter_time)
			{
				swap(exit_time, enter_time);
				normal[axis] = 1.f;
			}
		}

		if (enter_time > last_enter_time)
		{
			last_enter_time = enter_time;
			last_enter_normal = normal;
		}
		if (exit_time < first_exit_time)
			first_exit_time = exit_time;
	}

	// One axis exited before all of them entered, no overlap, no hit.
	if (first_exit_time < last_enter_time)
		return Hit_Result::make_no_hit(end);

	// Check if the hit is happening before the start-point or after the end-point
	if (first_exit_time < 0.f || last_enter_time > 1.f)
		return Hit_Result::make_no_hit(end);

	// Check if we're inside of the AABB, meaning we need to depenetrate
	if (first_exit_time > 0.f && last_enter_time < 0.f)
		return intersect_point(start, tar);

	return Hit_Result::make_hit(start + line_delta * last_enter_time, last_enter_normal, last_enter_time);
}

Hit_Result Collision::line_trace(const Vec3& start, const Vec3& end, const Sphere& tar)
{
	// First, check for penetration
	if (distance_sqrd(start, tar.origin) < Math::square(tar.radius))
	{
		// Calculate depth
		float dist_to_sphere = distance(tar.origin, start);
		Vec3 normal = direction_to(tar.origin, start);

		float depth = tar.radius - dist_to_sphere;

		return Hit_Result::make_penetrating(start, normal * depth);
	}

	// Find the closest point to sphere on the line
	Vec3 line_delta = end - start;
	Vec3 line_direction = direction_to(start, end);
	Vec3 offset = tar.origin - start;

	float closest_time = dot(line_direction, offset);

	// Uh oh, the closest point is behind us
	if (closest_time < 0.f)
		return Hit_Result::make_no_hit(end);

	Vec3 closest_point = start + line_direction * closest_time;

	// Is the closest point within the circle?
	if (distance_sqrd(closest_point, tar.origin) > Math::square(tar.radius))
		return Hit_Result::make_no_hit(end);

	// Okay, time to get the depth of the closest point and move back to when we acturally _entered_ the sphere
	float closest_origin_dist_sqrd = distance_sqrd(closest_point, tar.origin);
	float radius_sqrd = Math::square(tar.radius);

	float closest_depth = Math::sqrt(radius_sqrd - closest_origin_dist_sqrd);
	float enter_distance = closest_time - closest_depth;

	Vec3 enter_point = start + line_direction * enter_distance;
	Vec3 enter_normal = direction_to(tar.origin, enter_point);

	return Hit_Result::make_hit(enter_point, enter_normal, enter_distance / length(line_delta));
}

Hit_Result Collision::line_trace(const Vec3& start, const Vec3& end, const Capsule& tar)
{
	// First boil the problem down to 2 dimensions to determine
	//	the time to hit the infinite cylinder
	Vec3 parallel = normalize(tar.b - tar.a);
	float hit_time = 0.f;

	{
		Vec3 sphere_pos = constrain_to_plane(tar.a, parallel);
		Vec3 parallel_start = constrain_to_plane(start, parallel);
		Vec3 parallel_end = constrain_to_plane(end, parallel);

		Hit_Result parallel_hit = line_trace(parallel_start, parallel_end, Sphere(sphere_pos, tar.radius));

		// Well, we don't hit the infinite cylinder, so we won't hit the capsule either
		if (!parallel_hit.has_hit)
			return Hit_Result::make_no_hit(end);

		hit_time = parallel_hit.time;
	}
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

Hit_Result Collision::intersect_sphere(const Sphere& src, const Sphere& tar)
{
	Vec3 difference = src.origin - tar.origin;

	// Not intersecting
	if (length_sqrd(difference) > Math::square(src.radius + tar.radius))
		return Hit_Result::make_no_hit(src.origin);

	float depth = (src.radius + tar.radius) - length(difference);
	Vec3 normal = normalize(difference);

	return Hit_Result::make_penetrating(src.origin, normal * depth);
}

Hit_Result Collision::sweep_sphere(const Sphere& src, const Vec3& delta, const Sphere& tar)
{
	// Sphere sweeping is equal to a linetrace with a sphere thats inflated to the sum of both spheres
	Sphere inflated_target = Sphere(tar.origin, tar.radius + src.radius);
	return line_trace(src.origin, src.origin + delta, inflated_target);
}

Hit_Result Collision::sweep_sphere(const Sphere& src, const Vec3& delta, const AABB& tar)
{
	Hit_Result inflated_hit = line_trace(src.origin, src.origin + delta, tar.inflate(src.radius));

	// No hit on the inflated boy
	if (!inflated_hit.has_hit)
		return inflated_hit;

	// Okay so, we need to find out where the hit happened
	// We need to determine if we hit a face, an edge or a vertex
	Vec3 hit_pos = inflated_hit.position;
	Vec3 corner_vertex;
	Vec3 edge_vertex;
	u32 outside_edges = 0;

	for(u32 axis = 0; axis < 3; ++axis)
	{
		if (hit_pos[axis] < tar.min[axis])
		{
			corner_vertex[axis] = tar.min[axis];
			edge_vertex[axis] = tar.min[axis];
			outside_edges++;
		}
		else if (hit_pos[axis] > tar.max[axis])
		{
			corner_vertex[axis] = tar.max[axis];
			edge_vertex[axis] = tar.max[axis];
			outside_edges++;
		}
		else
		{
			corner_vertex[axis] = tar.min[axis];
			edge_vertex[axis] = tar.max[axis];
		}
	}

	// Being outside the boundaries of just one axis means we just hit a face! A line-trace was good enough
	if (outside_edges == 1)
		return inflated_hit;

	// Being ouside of 2 axes means we hit an edge
	if (outside_edges == 2)
	{
		debug->capsule(corner_vertex, edge_vertex, src.radius, Color::yellow);

		// Compress the problem to 2D, essentially tracing a line with a circle.
		// This will act like an infinite cylinder along the edge of the cube, but that should be fine!
		// There should be no way to hit the cylinder in an invalid spot (hopefully)

		Vec3 plane = normalize(edge_vertex - corner_vertex);
		Vec3 circle_point = constrain_to_plane(edge_vertex, plane);
		Vec3 start_point = constrain_to_plane(src.origin, plane);
		Vec3 end_point = constrain_to_plane(src.origin + delta, plane);

		// We use the hit here just for 
		// 1. Checking if there's a hit at all
		// 2. Getting the time of the hit
		Hit_Result edge_hit = line_trace(start_point, end_point, Sphere(circle_point, src.radius));
		if (!edge_hit.has_hit)
			return Hit_Result::make_no_hit(src.origin + delta);

		return Hit_Result::make_hit(src.origin + delta * edge_hit.time, edge_hit.normal, edge_hit.time);
	}

	// Being outside all axes means we hit a corner!
	if (outside_edges == 3)
	{
		debug->sphere(corner_vertex, src.radius, Color::yellow);
		// Alright so we hit a corner, lets do a linetrace with a sphere at that particular corner
		return line_trace(src.origin, src.origin + delta, Sphere(corner_vertex, src.radius));
	}

	debug->print(String::printf("Outside edges: %d", outside_edges));
	return inflated_hit;
}