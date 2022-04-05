#include "collider.h"
#include "debug/debug.h"

Hit_Result Collider::intersect(const Collider* other) const
{
	AABB src = as_aabb();
	AABB tar = other->as_aabb();

	return intersect_aabb(src, tar);
}

Hit_Result Collider::sweep(const Vec3& delta, const Collider* other) const
{
	AABB src = as_aabb();
	AABB tar = other->as_aabb();

	return sweep_aabb(src, delta, tar);
}

void Collider::debug_draw(const Color& clr, float thickness) const
{
	switch(shape)
	{
		case Shape_Type::AABB:
		{
			debug.box(position, size, Quat::identity, clr, thickness);
			return;
		}

		default:
			return;
	}
}