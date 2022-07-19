#pragma once
#include "container/array.h"
class Weapon;

class Player_Inventory
{
public:
	void init();
	Array<Weapon*> weapons;
};

extern Player_Inventory* inventory;