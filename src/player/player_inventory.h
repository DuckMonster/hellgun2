#pragma once
#include "container/array.h"
class Weapon;
class Weapon_Type;

class Player_Inventory
{
public:
	void init();
	void add_weapon(Weapon_Type* wpn);

	Array<Weapon*> weapons;
};

extern Player_Inventory* inventory;