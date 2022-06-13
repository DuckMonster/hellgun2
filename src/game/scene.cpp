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
	for(u32 i = 0; i < entities.count(); ++i)
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
	for(u32 i=0; i<colliders.count(); ++i)
	{
		if (colliders[i] == collider)
		{
			delete collider;
			colliders.remove_at_swap(i);
			return;
		}
	}
}

Hit_Result Scene::sweep(const Shape& shape, const Vec3& delta, Sweep_Info info)
{
	if (info.source_entity == NULL)
		info.ignore_self = false;

	Hit_Result result = Hit_Result::make_no_hit(shape.position + delta);

	for(auto* collider : colliders)
	{
		// Collider not covered by the object mask
		if ((collider->object_type & info.object_mask) == 0)
			continue;

		// Ignore self
		if (info.ignore_self && info.source_entity == collider->owner)
			continue;

		Hit_Result hit = collider->receive_sweep(shape, delta);
		result = Collision::select_first_hit(result, hit);
	}

	return result;
}

Array<Hit_Result> Scene::sweep_multi(const Shape& shape, const Vec3& delta, Sweep_Info info)
{
	if (info.source_entity == NULL)
		info.ignore_self = false;

	Array<Hit_Result> results;
	for(auto* collider : colliders)
	{
		// Collider not covered by the object mask
		if ((collider->object_type & info.object_mask) == 0)
			continue;

		// Ignore self
		if (info.ignore_self && info.source_entity == collider->owner)
			continue;

		Hit_Result hit = collider->receive_sweep(shape, delta);
		if (hit.has_hit)
			results.add(hit);
	}

	return results;
}

Hit_Result Scene::sweep_collider(const Collider* collider, const Vec3& delta, Sweep_Info info)
{
	Hit_Result result = Hit_Result::make_no_hit(collider->position + delta);

	for(auto* other_collider : colliders)
	{
		// Collider not covered by the object mask
		if ((other_collider->object_type & info.object_mask) == 0)
			continue;

		// Ignore self
		if (other_collider == collider)
			continue;

		Hit_Result hit = collider->sweep(delta, other_collider);
		result = Collision::select_first_hit(result, hit);
	}

	return result;
}

Drawable* Scene::add_drawable()
{
	Drawable* drawable = new Drawable();
	drawables.add(drawable);

	return drawable;
}

void Scene::destroy_drawable(Drawable* drawable)
{
	drawables.remove(drawable);
	delete drawable;
}

void Scene::add_damage_number(const Vec3& position, float damage, const Vec3& direction)
{
	// Find if there's an inactive number we can re-use
	for(auto& nmbr : damage_numbers)
	{
		if (!nmbr.active)
		{
			nmbr.begin(position, damage, direction);
			return;
		}
	}

	// Nope, we need to add a new one
	Damage_Number& number = damage_numbers.add_default();
	number.init();
	number.begin(position, damage, direction);
}