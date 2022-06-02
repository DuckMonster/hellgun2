#include "pistol.h"
#include "core/input.h"
#include "game/scene.h"
#include "game/game.h"
#include "player/player.h"
#include "pistolprojectile.h"

void Pistol::update()
{
	if (ammo > 0 && mouse_pressed(Mouse_Btn::Left))
	{
		// Fire projectile
		Vec3 spawn_pos = game->player->position;
		Vec3 target_pos = game->get_mouse_game_position();
		Vec3 direction = direction_to(spawn_pos, target_pos);

		spawn_pos += direction * 1.f;

		Pistol_Projectile* projectile = scene->spawn_entity<Pistol_Projectile>(spawn_pos);
		projectile->host = this;

		projectile->velocity = direction * 800.f;

		// Apply impulse to player
		game->player->velocity -= direction * 25.f;

		ammo--;
	}
}

void Pistol::render(const Render_Info& info)
{

}