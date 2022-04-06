#pragma once
#include "aabb.h"
#include "collision.h"
class Entity;

enum class Shape_Type
{
	Invalid,
	AABB,
};

class Collider
{
public:
	Entity* owner = nullptr;
	Vec3 position;

	Shape_Type shape = Shape_Type::Invalid;
	Vec3 size;

	void set_aabb(const Vec3& box_size)
	{
		shape = Shape_Type::AABB;
		size = box_size;
	}
	AABB as_aabb() const
	{
		return AABB::from_center_size(position, size);
	}

	Hit_Result intersect(const Collider* other) const;
	Hit_Result sweep_to(const Vec3& delta, const Collider* target) const;
	Hit_Result sweep_test(const AABB& src, const Vec3& delta) const;

	void debug_draw(const Color& clr, float thickness = 2.f) const;
};