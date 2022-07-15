#include "item_bank.h"
#include "weapon/pistol/pistol.h"
#include "weapon/cross/cross.h"

Array<Weapon_Info> weapon_bank;
template<typename TWeapon>
void register_weapon(const TString& name)
{
	weapon_bank.add({name, []() -> Weapon* { return new TWeapon(); }});
}

void register_all_items()
{
	register_weapon<Pistol>("Pistol");
	register_weapon<Cross>("Cross");
}