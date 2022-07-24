#include "item_bank.h"
#include "weapon/pistol/pistol.h"
#include "weapon/cross/cross.h"

Array<Weapon_Type*> weapon_bank;
template<typename TWeapon>
void register_weapon()
{
	weapon_bank.add(TWeapon::static_type());
}

void register_all_items()
{
	register_weapon<Pistol>();
	register_weapon<Cross>();
}