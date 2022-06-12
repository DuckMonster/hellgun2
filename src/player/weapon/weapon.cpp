#include "weapon.h"
#include "game/game.h"
#include "player/player.h"

Vec3 Weapon::get_aim_direction()
{
	Vec3 target_pos = game->get_mouse_game_position();
	return direction_to(game->player->position, target_pos);
}