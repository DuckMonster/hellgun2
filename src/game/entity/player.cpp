#include "player.h"
#include "core/input.h"
#include "debug/debug.h"
#include "game/game.h"
#include "game/scene.h"
#include "resource/resourcecommon.h"
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

	if (key_down(Key::Spacebar))
	{
		velocity.y = 10.f;
	}

	velocity.x += input.x * PLAYER_WALK_ACC * time_delta();
	velocity += -Vec3::up * PLAYER_GRAVITY * time_delta();
	velocity -= velocity * PLAYER_FRICTION * time_delta();

	// Collision tracing
	Vec3 delta_to_move = velocity * time_delta();
	int it = 0;

	while(!is_nearly_zero(delta_to_move) && ++it < 10)
	{
		AABB box = AABB::from_center_size(position, Vec3::one);
		Hit_Result hit = scene.sweep_aabb(box, delta_to_move);

		if (hit.is_penetrating)
		{
			printf("PENETRATE\n");
			position += hit.normal * hit.penetration_depth;
			continue;
		}

		if (hit.has_hit)
		{
			position = hit.position;
			velocity = constrain_to_plane(velocity, hit.normal);
			delta_to_move = constrain_to_plane(delta_to_move, hit.normal);

			delta_to_move = delta_to_move * (1.f - hit.time);
			continue;
		}

		position += delta_to_move;
		break;
	}

	// Ground check
	Hit_Result hit = scene.sweep_aabb(AABB::from_center_size(position, Vec3::one), Vec3(0.f, -0.01f, 0.f));
	if (hit.collider->owner)
	{
		printf("Standing on something!!\n");
	}
}

void Player::render(const Render_Info& info)
{
	auto& mat = Common_Mat::test;

	mat.use();
	mat.set("u_ViewProjection", info.view_projection);
	mat.set("u_Model", mat_translation(position));

	auto& mesh = Common_Mesh::rect;
	mesh.bind();
	mesh.draw();
}