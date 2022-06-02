#include "shape.h"
#include "debug/debug.h"

Hit_Result Shape::intersect(const Shape& target) const
{
	return Hit_Result::make_no_hit(position);
}

Hit_Result Shape::sweep(const Vec3& delta, const Shape& target) const
{
	using namespace Collision;
	switch(type)
	{
		case Type::Point:
		{
			switch(target.type)
			{
				case Type::AABB: return line_trace(position, position + delta, target.as_aabb());
				case Type::Sphere: return line_trace(position, position + delta, target.as_sphere());
			}
			break;
		}

		case Type::AABB:
		{
			switch(target.type)
			{
				case Type::AABB: return sweep_aabb(as_aabb(), delta, target.as_aabb());
				case Type::Sphere: return sweep_aabb(as_aabb(), delta, target.as_sphere());
			}
			break;
		}

		case Type::Sphere:
		{
			switch(target.type)
			{
				case Type::AABB: return sweep_sphere(as_sphere(), delta, target.as_aabb());
				case Type::Sphere: return sweep_sphere(as_sphere(), delta, target.as_sphere());
			}
			break;
		}
	}

	debug->print(String::printf("Shape sweep failed (shape type %d / %d)", (u32)type, (u32)target.type));
	return Hit_Result::make_no_hit(position + delta);
}

void Shape::debug_draw(const Color& clr) const
{
	switch(type)
	{
		case Type::AABB:
			debug->box(position, size, Quat::identity, clr);
			break;

		case Type::Sphere:
			debug->sphere(position, size.x, clr);
			break;
	}
}