#pragma once
#include "collision/collider.h"
#include "container/array.h"

class Entity;
struct Mesh;

struct Sweep_Info
{
	Entity* source_entity = NULL;
	bool ignore_self = false;

	Collider_Object_Type object_mask = COBJ_All;
};

struct Scene
{
public:
	Array<Entity*> entities;
	bool pending_destruction = false;

	template<typename T>
	T* spawn_entity(const Vec3& position)
	{
		T* entity = new T();
		entities.add(entity);

		entity->spawn_time = time_elapsed();
		entity->position = position;
		entity->init();

		return entity;
	}

	void destroy_entity(Entity* entity);
	void finish_destruction();

	template<typename T>
	T* sphere_find_entity(const Vec3& origin, float radius)
	{
		for(auto* entity : entities)
		{
			if (!entity->IsA<T>())
				continue;

			float dist_sqrd = distance_sqrd(origin, entity->position);
			if (dist_sqrd < Math::square(radius))
				return (T*)entity;
		}

		return nullptr;
	}

	Array<Collider*> colliders;
	Collider* add_collider();
	void destroy_collider(Collider* collider);

	Hit_Result sweep(const Shape& shape, const Vec3& delta, Sweep_Info info = Sweep_Info());
	Hit_Result sweep_collider(const Collider* collider, const Vec3& delta, Sweep_Info info = Sweep_Info());
};
extern Scene* scene;