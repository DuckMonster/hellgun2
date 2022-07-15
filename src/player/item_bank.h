#pragma once
#include "core/delegate.h"
#include "container/array.h"
class Weapon;

struct Weapon_Info
{
	String name;
	Delegate<Weapon*> instantiate;
};
extern Array<Weapon_Info> weapon_bank;

void register_all_items();