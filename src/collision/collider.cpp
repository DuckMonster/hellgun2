#include "collider.h"
#include "debug/debug.h"

Hit_Result Collider::intersect(const Collider* other) const
{
	AABB src = as_aabb();
	AABB tar = other->as_aabb();

	Hit_Result hit = Collision::intersect_aabb(src, tar);
	hit.collider = other;

	return hit;
}

Hit_Result Collider::sweep_to(const Vec3& delta, const Collider* target) const
{
	AABB src = as_aabb();
	return target->sweep_test(src, delta);
}

Hit_Result Collider::sweep_test(const AABB& src, const Vec3& delta) const
{
	Hit_Result hit;
	switch(shape)
	{
		case Shape_Type::AABB:
			hit = Collision::sweep_aabb(src, delta, as_aabb());
			break;
	}

	hit.collider = this;
	return hit;
}

void Collider::debug_draw(const Color& clr, float thickness) const
{
	switch(shape)
	{
		case Shape_Type::AABB:
			debug->box(position, size, Quat::identity, clr, thickness);
			return;

		case Shape_Type::Sphere:
			debug->sphere(position, size.x, clr, thickness);
			return;
	}
}