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
static Entity_Class* StaticClass()\
{\
	static Entity_Class cls(#ClassName);\
	return &cls;\
}\
\
Entity_Class* GetClass() override\
{\
	return ClassName::StaticClass();\
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

	virtual Entity_Class* GetClass() { return nullptr; };

	template<typename T>
	bool IsA()
	{
		return GetClass() == T::StaticClass();
	}
};