#include "cross.h"
#include "core/input.h"
#include "cross_projectile.h"
#include "game/game.h"
#include "game/scene.h"
#include "player/player.h"
#include "gfx/mesh.h"
#include "gfx/material.h"
#include "gfx/texture.h"
#include "resource/resource.h"

void Cross::on_equipped()
{
	position = game->player->position;
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
}

void Cross::render(const Render_Info& info)
{
	if (ammo == 0)
		return;

	Mesh* mesh = Resource::load_mesh("mesh/plane.msh");
	Material* mat = Resource::load_material("material/sprite.mat");
	Texture* tex = Resource::load_texture("texture/cross.tga");

	mat->use();
	mat->set("u_ViewProjection", info.view_projection);
	mat->set("u_Model", mat_translation(position) * mat_orient_x(get_aim_direction()) * mat_scale(2.f));

	tex->bind();
	mesh->draw();
}