#pragma once
#include "sphere.h"
#include "aabb.h"
#include "collision.h"

struct Shape
{
public:
	enum class Type
	{
		Invalid,
		Point,
		AABB,
		Sphere,
		MAX,
	};

	static Shape point(const Vec3& position) { return Shape(Type::Point, position, Vec3::zero); }
	static Shape aabb(const Vec3& center, const Vec3& size) { return Shape(Type::AABB, center, size); }
	static Shape sphere(const Vec3& center, float radius) { return Shape(Type::Sphere, center, Vec3(radius)); }

	Type type;
	Vec3 position;
	Vec3 size;

	Shape() : type(Type::Invalid), position(), size() {}
	Shape(Type type, const Vec3& position, const Vec3& size)
		:type(type), position(position), size(size) {}

	AABB as_aabb() const { return AABB::from_center_size(position, size); }
	Sphere as_sphere() const { return Sphere(position, size.x); }

	Hit_Result intersect(const Shape& target) const;
	Hit_Result sweep(const Vec3& delta, const Shape& target) const;

	void debug_draw(const Color& clr) const;
};