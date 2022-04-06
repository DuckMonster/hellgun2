#include "scene.h"
#include <stdio.h>
#include "entity/entity.h"

Scene scene;

Collider* Scene::add_collider()
{
	Collider* collider = new Collider();
	colliders.add(collider);

	return collider;
}

Hit_Result Scene::sweep_aabb(const AABB& src, const Vec3& delta, Sweep_Info info)
{
	if (info.source_entity == NULL)
		info.ignore_self = false;

	static bool retry = false;

	Hit_Result hit = Hit_Result::make_no_hit(src.center() + delta);

	for(auto* collider : colliders)
	{
		// Ignore self
		if (info.ignore_self && info.source_entity == collider->owner)
			continue;

		hit = Collision::select_first_hit(hit, collider->sweep_test(src, delta));
	}

	if (hit.contains_nan() && !retry)
	{
		printf("sweep resulted in NaN\n");

		retry = true;
		sweep_aabb(src, delta);
		retry = false;
	}

	return hit;
}