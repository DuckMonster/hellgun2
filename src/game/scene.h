#pragma once
#include "collision/collider.h"
#include "container/array.h"

class Entity;

struct Sweep_Info
{
	Entity* source_entity = NULL;
	bool ignore_self = false;
};

struct Scene
{
public:
	Array<Entity*> entities;
	template<typename T>
	T* spawn_entity(const Vec3& position)
	{
		T* entity = new T();
		entities.add(entity);

		entity->position = position;
		entity->init();

		return entity;
	}

	Array<Collider*> colliders;
	Collider* add_collider();

	Hit_Result sweep_aabb(const AABB& src, const Vec3& delta, Sweep_Info info = Sweep_Info());
};
extern Scene scene;
