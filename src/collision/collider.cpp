#include "collider.h"
#include "debug/debug.h"

Shape Collider::get_transformed_shape(u32 index) const
{
	if (index > shapes.count())
	{
		error("get_transformed_shape out of range (%d / %d)", index, shapes.count());
		return Shape();
	}

	Shape shape = shapes[index];
	shape.position += position; // :^) no matrices here

	return shape;
}

Hit_Result Collider::sweep(const Vec3& delta, const Collider* other) const
{
	Hit_Result result = Hit_Result::make_no_hit(position + delta);

	for(u32 i = 0; i < num_shapes(); ++i)
	{
		Shape my_shape = get_transformed_shape(i);
		Hit_Result hit = other->receive_sweep(my_shape, delta);

		result = Collision::select_first_hit(result, hit);
	}

	return result;
}

Hit_Result Collider::receive_sweep(const Shape& shape, const Vec3& delta) const
{
	Hit_Result result = Hit_Result::make_no_hit(shape.position + delta);

	for(u32 i = 0; i < num_shapes(); ++i)
	{
		Shape my_shape = get_transformed_shape(i);
		Hit_Result hit = shape.sweep(delta, my_shape);

		result = Collision::select_first_hit(result, hit);
	}

	result.collider = this;
	return result;
}

void Collider::debug_draw(const Color& clr, float thickness) const
{
	for(u32 i = 0; i < num_shapes(); ++i)
		get_transformed_shape(i).debug_draw(clr);
}