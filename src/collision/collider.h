#pragma once
#include "aabb.h"
#include "collision.h"
class Entity;

enum Collider_Object_Type
{
	COBJ_Player = 1 << 0,
	COBJ_Enemy = 1 << 1,
	COBJ_World = 1 << 2,

	COBJ_All = 0xF
};

enum class Shape_Type
{
	Invalid,
	AABB,
	Sphere,
};

class Collider
{
public:
	Entity* owner = nullptr;
	Vec3 position;

	Collider_Object_Type object_type;

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
	void set_sphere(float radius)
	{
		shape = Shape_Type::Sphere;
		size.x = radius;
	}
	Sphere as_sphere() const
	{
		return Sphere(position, size.x);
	}

	Hit_Result intersect(const Collider* other) const;
	Hit_Result sweep_to(const Vec3& delta, const Collider* target) const;
	Hit_Result sweep_test(const AABB& src, const Vec3& delta) const;

	void debug_draw(const Color& clr, float thickness = 2.f) const;
};