#include "player_inventory.h"
#include "player/weapon/weapon.h"

void Player_Inventory::init()
{
	weapons.set_count(4, nullptr);
}

void Player_Inventory::add_weapon(Weapon_Type* wpn)
{
	for(u32 i = 0; i < weapons.count(); ++i)
	{
		if (weapons[i] == nullptr)
		{
			weapons[i] = wpn->create_new();
			weapons[i]->init();
			break;
		}
	}
}

Player_Inventory* inventory;