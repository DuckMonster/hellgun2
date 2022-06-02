#pragma once
#include "aabb.h"
#include "collision.h"
#include "shape.h"
#include "container/array.h"
class Entity;

enum Collider_Object_Type
{
	COBJ_Player = 1 << 0,
	COBJ_Enemy = 1 << 1,
	COBJ_World = 1 << 2,

	COBJ_All = 0xF
};

class Collider
{
public:
	Entity* owner = nullptr;
	Collider_Object_Type object_type = COBJ_All;

	Vec3 position;

	Array<Shape> shapes;
	void attach_shape(const Shape& shape) { shapes.add(shape); }
	u32 num_shapes() const { return shapes.count(); }
	Shape get_transformed_shape(u32 index) const;

	Hit_Result sweep(const Vec3& delta, const Collider* other) const;
	Hit_Result receive_sweep(const Shape& shape, const Vec3& delta) const;

	void debug_draw(const Color& clr, float thickness = 2.f) const;
};