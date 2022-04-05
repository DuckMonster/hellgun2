#pragma once
#include "collision/collider.h"
#include "container/array.h"

struct Scene
{
public:
	Collider* add_collider();

	Hit_Result sweep_aabb(const Vec3& start, const Vec3& end, const Vec3& size);
	Array<Collider> colliders;
};
extern Scene scene;
