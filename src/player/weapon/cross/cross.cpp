#include "cross.h"
#include "core/input.h"
#include "cross_projectile.h"
#include "game/game.h"
#include "game/scene.h"
#include "player/player.h"
#include "game/drawable.h"

void Cross::init()
{
	mesh = scene->add_drawable();
	mesh->load("mesh/plane.msh", "material/sprite.mat", "texture/cross.tga");
	mesh->set_disabled(true);
}

void Cross::on_equipped()
{
	position = game->player->position;
	mesh->set_disabled(false);
}

void Cross::on_unequipped()
{
	mesh->set_disabled(true);
}

void Cross::update()
{
	if (mouse_pressed(Mouse_Btn::Left) && ammo > 0)
	{
		Vec3 spawn_pos = position + get_aim_direction() * 1.f;
		Vec3 direction = get_aim_direction();

		// Give some extra force in case the player is moving forwards
		float extra_impulse = dot(game->player->velocity, direction);

		// Don't give negative extra impulse...
		if (extra_impulse < 0.f)
			extra_impulse = 0.f;

		Cross_Projectile* proj = scene->spawn_entity<Cross_Projectile>(spawn_pos);
		proj->host = this;
		proj->velocity = direction * (CROSS_THROW_IMPULSE + extra_impulse);

		ammo--;
	}

	Vec3 target_position = game->player->position - get_aim_direction() * 1.f;
	position = Math::lerp(position, target_position, 45.f * time_delta());

	mesh->matrix = mat_translation(position) * mat_orient_x(get_aim_direction()) * mat_scale(2.f);
}