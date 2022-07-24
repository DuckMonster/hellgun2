#pragma once
#include "core/delegate.h"
#include "container/array.h"

class Weapon_Type;
extern Array<Weapon_Type*> weapon_bank;

void register_all_items();