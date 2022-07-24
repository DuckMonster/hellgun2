#pragma once
#include "gfx/render_info.h"

class Weapon;
class Weapon_Type
{
public:
	String name;
	String description;
	String icon_path;

	Weapon_Type()
	{
	}

	virtual Weapon* create_new() = 0;
};

#define WEAPON_BEGIN(type)\
public:\
Weapon_Type* get_type() override { return type::static_type(); }\
static Weapon_Type* static_type()\
{\
	static Weapon_Type_##type _type;\
	return &_type;\
}\
private:\
class Weapon_Type_##type : public Weapon_Type\
{\
public:\
	Weapon* create_new() override { return new type(); }\
	Weapon_Type_##type()\
	{\
		name = #type;\
		description = "[NO DESCRIPTION]";\
		icon_path = "texture/skull_whole.tga";\

#define WEAPON_NAME(_name)\ name = _name;
#define WEAPON_DESCRIPTION(_desc) description = _desc;
#define WEAPON_ICON(_icon) icon_path = _icon;

#define WEAPON_END()\
	}\
};\

class Weapon
{
public:
	virtual void init() {}
	virtual void on_equipped() {}
	virtual void on_unequipped() {}

	virtual void update() {}
	virtual void render(const Render_Info& info) {}

	virtual Weapon_Type* get_type() = 0;

	Vec3 get_aim_direction();
};