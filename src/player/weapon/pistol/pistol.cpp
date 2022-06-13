#include "pistol.h"
#include "core/input.h"
#include "game/scene.h"
#include "game/game.h"
#include "player/player.h"
#include "pistol_projectile.h"
#include "fx/fx.h"
#include "fx/surface_impact_system.h"
#include "game/drawable.h"

void Pistol::init()
{
	mesh = scene->add_drawable();
	mesh->load("mesh/plane.msh", "material/sprite.mat", "texture/pistol_hold.tga");
	mesh->set_disabled(true);
}

void Pistol::on_equipped()
{
	position = game->player->position;
	mesh->set_disabled(false);
}

void Pistol::on_unequipped()
{
	mesh->set_disabled(true);
}

void Pistol::update()
{
	if (ammo > 0 && mouse_pressed(Mouse_Btn::Left))
	{
		// Fire projectile
		Vec3 spawn_pos = position + get_aim_direction() * 1.f;
		Vec3 direction = get_aim_direction();

		Pistol_Projectile* projectile = scene->spawn_entity<Pistol_Projectile>(spawn_pos);
		projectile->host = this;

		projectile->velocity = direction * 800.f;

		// Shoot FX
		fx->spawn_system<Surface_Impact_System>(spawn_pos, direction, direction, direction);

		// Apply impulse to player
		game->player->velocity -= direction * 25.f;

		// Apply recoil
		recoil_offset -= direction * 2.f;
		recoil_angle = 1.2f;

		ammo--;
	}

	// Position the gun
	Vec3 target_position = game->player->position + get_aim_direction() * 2.f;
	position = Math::lerp(position, target_position, 45.f * time_delta());

	// reduce recoil
	recoil_angle = Math::lerp(recoil_angle, 0.f, 12.f * time_delta());
	recoil_offset = Math::lerp(recoil_offset, Vec3::zero, 7.f * time_delta());

	Quat recoil_quat = Quat(Vec3::right, recoil_angle);
	mesh->matrix =
		mat_translation(position + recoil_offset) *
		mat_orient_x(get_aim_direction()) *
		recoil_quat.matrix() *
		mat_scale(2.f);
}