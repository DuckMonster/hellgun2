#pragma once
#include "gfx/renderinfo.h"

class Entity_Class
{
public:
	const char* const name;
	Entity_Class(const char* name)
		: name(name)
	{
	}
};

#define ENTITY_CLASS(ClassName)\
public:\
static Entity_Class* static_class()\
{\
	static Entity_Class cls(#ClassName);\
	return &cls;\
}\
\
Entity_Class* get_class() override\
{\
	return ClassName::static_class();\
}\
\
private:

class Entity
{
public:
	Vec3 position;
	float spawn_time;

	bool marked_for_destroy = false;

	float get_age() { return time_since(spawn_time); }

	virtual void init() {}
	virtual void on_destroyed() {}

	virtual void update() {}
	virtual void render(const Render_Info& info) {}

	virtual Entity_Class* get_class() { return nullptr; };

	template<typename T>
	bool is_a()
	{
		return get_class() == T::static_class();
	}
};

template<typename T>
T* cast(Entity* entity)
{
	if (!entity)
		return nullptr;
	if (!entity->is_a<T>())
		return nullptr;

	return (T*)entity;
}