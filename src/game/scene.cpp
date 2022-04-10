#include "scene.h"
#include <stdio.h>
#include "entity/entity.h"

Scene* scene;

void Scene::destroy_entity(Entity* entity)
{
	entity->marked_for_destroy = true;
	pending_destruction = true;
}

void Scene::finish_destruction()
{
	for(int i = 0; i < entities.count(); ++i)
	{
		if (entities[i]->marked_for_destroy)
		{
			entities[i]->on_destroyed();
			delete entities[i];

			entities.remove_at_swap(i);
			--i;
		}
	}

	pending_destruction = false;
}

Collider* Scene::add_collider()
{
	Collider* collider = new Collider();
	colliders.add(collider);

	return collider;
}

void Scene::destroy_collider(Collider* collider)
{
	for(int i=0; i<colliders.count(); ++i)
	{
		if (colliders[i] == collider)
		{
			delete collider;
			colliders.remove_at_swap(i);
			return;
		}
	}
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