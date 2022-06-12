#include "cross_projectile.h"
#include "cross.h"
#include "gfx/mesh.h"
#include "gfx/material.h"
#include "gfx/texture.h"
#include "resource/resource.h"
#include "game/game.h"
#include "game/scene.h"
#include "player/player.h"
#include "entity/enemy/enemy.h"
#include "fx/fx.h"
#include "fx/weapon/impact_spike_system.h"
#include "debug/debug.h"

void Cross_Projectile::init()
{
	last_sweep_position = position;
	next_sweep_time = time_elapsed() + CROSS_SWEEP_INTERVAL;
}

void Cross_Projectile::on_destroyed()
{
	host->ammo++;
}

void Cross_Projectile::update()
{
	// Normal physics
	velocity -= velocity * CROSS_FRICTION * time_delta();

	// Perform pullback force
	Vec3 dir_to_player = direction_to(position, game->player->position);
	velocity += dir_to_player * CROSS_PULLBACK_STR * time_delta();

	// Perform damage sweep
	if (time_has_reached(next_sweep_time))
	{
		Shape shape = Shape::sphere(last_sweep_position, 2.5f);
		Sweep_Info info;
		info.source_entity = this;
		info.object_mask = COBJ_Enemy;

		Array<Hit_Result> hits = scene->sweep_multi(shape, position - last_sweep_position, info);
		for(auto& hit : hits)
		{
			Enemy* hit_enemy = cast<Enemy>(hit.collider->owner);
			if (hit_enemy)
			{
				Vec3 dir_to_enemy = direction_to(position, hit_enemy->position);

				Damage_Data data;
				data.damage = 2.f;
				data.visual_scale = 2.f;
				data.direction = dir_to_enemy;
				data.impulse = dir_to_enemy * 2.f;

				hit_enemy->hit(data);
			}
		}

		last_sweep_position = position;
		next_sweep_time = time_elapsed() + CROSS_SWEEP_INTERVAL;
	}

	// So, we want to rotate the velocity towards the player, to make sure that they catch it
	if (get_age() > 1.f)
	{
		float pullback_age = get_age() - 1.f;

		Vec3 velocity_toward_player = dir_to_player * length(velocity);
		velocity = Math::lerp(velocity, velocity_toward_player, Math::min(pullback_age * 25.5f * time_delta(), 1.f));

		if (distance_sqrd(position, game->player->position) < Math::square(3.f))
		{
			game->player->velocity *= 0.1f;
			game->player->velocity += dir_to_player * CROSS_CATCH_IMPULSE;

			scene->destroy_entity(this);
			return;
		}
	}

	// Do sweeping against geometry
	// (but only for a while, after that we want to guarantee the projectile coming back)
	if (get_age() < 1.5f)
	{
		Shape shape = Shape::sphere(position, 2.5f);
		Sweep_Info info;
		info.object_mask = COBJ_World;

		Hit_Result hit = scene->sweep(shape, velocity * time_delta(), info);
		if (hit.has_hit)
		{
			if (hit.is_penetrating)
				position += hit.normal * (hit.penetration_depth + 0.1f);
			else
				velocity -= constrain_to_direction(velocity, hit.normal) * 1.6f;
		}
	}

	position += velocity * time_delta();
}

void Cross_Projectile::render(const Render_Info& info)
{
	Mesh* mesh = Resource::load_mesh("mesh/plane.msh");
	Material* mat = Resource::load_material("material/sprite.mat");
	Texture* tex = Resource::load_texture("texture/cross.tga");

	mat->use();
	mat->set("u_ViewProjection", info.view_projection);
	mat->set("u_Model", mat_translation(position) * Quat(Vec3::right, CROSS_ROTATE_SPEED * get_age()).matrix() * mat_scale(5.f));

	tex->bind();
	mesh->draw();
}