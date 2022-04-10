#include "player.h"
#include "core/input.h"
#include "debug/debug.h"
#include "game/game.h"
#include "game/scene.h"
#include "resource/resourcecommon.h"
#include "bullet.h"
#include "math/plane.h"
#include "ammodrop.h"
#include "resource/resource.h"
#include <stdio.h>

void Player::init()
{
}

void Player::update()
{
	Vec3 input = Vec3::zero;
	if (key_down(Key::A))
		input.x -= 1.f;
	if (key_down(Key::D))
		input.x += 1.f;

	if (mouse_pressed(Mouse_Btn::Left) && ammo > 0)
	{
		Vec3 aim_position = calculate_aim_position();
		Vec3 direction = normalize(aim_position - position);

		Bullet* bullet = scene->spawn_entity<Bullet>(position);
		bullet->velocity = direction * 120.f;

		velocity += -direction * 20.f;
		ammo--;

		debug->print("POW!", 2.f);
	}

	update_movement();
	move(velocity * time_delta());

	// Check for ammo drops
	Ammo_Drop* ammo = scene->sphere_find_entity<Ammo_Drop>(position, 1.f);
	if (ammo)
	{
		gain_ammo();
		scene->destroy_entity(ammo);
	}
}

void Player::update_movement()
{
	// Update grounded
	if (is_grounded)
	{
		AABB box = AABB::from_center_size(position, Vec3(1.f));
		Hit_Result hit = scene->sweep_aabb(box, Vec3(0.f, -0.1f, 0.f));

		// No ground under us, we're airborne!
		if (!hit.has_hit)
			is_grounded = false;
	}
	else
	{
		// Only re-ground ourselves if we're actully moving downwards
		if (velocity.y < 1.2f)
		{
			AABB box = AABB::from_center_size(position, Vec3(1.f));
			Hit_Result hit = scene->sweep_aabb(box, Vec3(0.f, -0.1f, 0.f));

			if (hit.has_hit)
			{
				position = hit.position;
				is_grounded = true;
			}
		}
	}

	if (is_grounded)
		update_grounded();
	else
		update_airborne();

	// Update friction scale
	friction_scale = Math::lerp(friction_scale, 1.f, 2.5f * time_delta());

	// Dashing
	if (key_pressed(Key::LeftShift) && get_movement_input() != 0.f)
	{
		velocity = Vec3(1.f, 0.f, 0.f) * get_movement_input() * PLAYER_GROUND_DASH_IMPULSE;
		friction_scale = 0.f;
	}
}

void Player::update_grounded()
{
	// Move input
	velocity.x += get_movement_input() * PLAYER_GROUND_ACC * friction_scale * time_delta();

	// Friction
	velocity -= velocity * PLAYER_GROUND_FRICTION * friction_scale * time_delta();

	// Jumping
	air_jumps = 1;
	if (key_pressed(Key::Spacebar))
		velocity.y = PLAYER_JMP_IMPULSE;
}

void Player::update_airborne()
{
	// Move input
	velocity.x += get_movement_input() * PLAYER_AIR_ACC * friction_scale * time_delta();

	// Gravity
	velocity -= Vec3::up * PLAYER_GRAVITY * friction_scale * time_delta();

	// Friction
	velocity.x -= velocity.x * PLAYER_AIR_FRICTION_H * friction_scale * time_delta();
	velocity.y -= velocity.y * PLAYER_AIR_FRICTION_V * friction_scale * time_delta();

	if (key_pressed(Key::Spacebar) && air_jumps > 0)
	{
		velocity.y = PLAYER_AIR_JMP_IMPULSE;
		air_jumps--;
	}
}

void Player::move(Vec3 delta)
{
	int it = 0;
	while(!is_nearly_zero(delta) && ++it < 10)
	{
		AABB box = AABB::from_center_size(position, Vec3::one);
		Hit_Result hit = scene->sweep_aabb(box, delta);

		if (hit.is_penetrating)
		{
			position += hit.normal * hit.penetration_depth;
			continue;
		}

		if (hit.has_hit)
		{
			position = hit.position;
			velocity = constrain_to_plane(velocity, hit.normal);
			delta = constrain_to_plane(delta, hit.normal);

			delta = delta * (1.f - hit.time);
			continue;
		}

		position += delta;
		return;
	}
}

void Player::render(const Render_Info& info)
{
	Material* mat = Resource::load_material("res/shader/test.vert", "res/shader/test.frag");

	mat->use();
	mat->set("u_ViewProjection", info.view_projection);
	mat->set("u_Model", mat_translation(position));

	auto& mesh = Common_Mesh::rect;
	mesh.bind();
	mesh.draw();
}

Vec3 Player::calculate_aim_position()
{
	Ray mouse_ray = game->get_mouse_world_ray();
	Plane game_plane = Plane(Vec3::zero, Vec3(0.f, 0.f, 1.f));

	Vec3 aim_position;
	mouse_ray.intersect(game_plane, &aim_position);

	return aim_position;
}

float Player::get_movement_input()
{
	float input = 0.f;
	if (key_down(Key::D))
		input += 1.f;
	if (key_down(Key::A))
		input -= 1.f;

	return input;
}