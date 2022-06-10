#include "cross.h"
#include "core/input.h"
#include "cross_projectile.h"
#include "game/game.h"
#include "game/scene.h"
#include "player/player.h"

void Cross::update()
{
	if (mouse_pressed(Mouse_Btn::Left))
	{
		Vec3 spawn_pos = game->player->position;
		Vec3 target_pos = game->get_mouse_game_position();
		Vec3 direction = direction_to(spawn_pos, target_pos);

		// Give some extra force in case the player is moving forwards
		float extra_impulse = dot(game->player->velocity, direction);

		// Don't give negative extra impulse...
		if (extra_impulse < 0.f)
			extra_impulse = 0.f;

		Cross_Projectile* proj = scene->spawn_entity<Cross_Projectile>(spawn_pos);
		proj->host = this;
		proj->velocity = direction * (CROSS_THROW_IMPULSE + extra_impulse);
	}
}

void Cross::render(const Render_Info& info)
{

}