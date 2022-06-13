#include "enemy.h"
#include "player/player.h"
#include "game/game.h"
#include "game/scene.h"
#include "debug/debug.h"
#include "fx/fx.h"
#include "fx/enemy/enemy_damage_system.h"
#include "fx/weapon/impact_spike_system.h"
#include "math/random.h"

void Enemy::init()
{
	acceleration = Random::range(ENEMY_ACC - 5.f, ENEMY_ACC + 5.f);
	friction = Random::range(ENEMY_FRICTION - 0.2f, ENEMY_FRICTION + 0.2f);

	collider = scene->add_collider();
	collider->object_type = COBJ_Enemy;
	collider->attach_shape(Shape::sphere(Vec3::zero, 1.5f));
	collider->owner = this;
	collider->position = position;

	skull = scene->add_drawable();
	skull->load("mesh/plane.msh", "material/sprite.mat", "texture/skull.tga");
	skull->attach_to(this);
	skull->matrix = mat_scale(3.f);
	skull->depth_offset = 0.f;

	jaw_front = scene->add_drawable();
	jaw_front->load("mesh/plane.msh", "material/sprite.mat", "texture/skull_jawf.tga");
	jaw_front->attach_to(this);
	jaw_front->matrix = mat_scale(3.f);
	jaw_front->depth_offset = -2.f;

	jaw_back = scene->add_drawable();
	jaw_back->load("mesh/plane.msh", "material/sprite.mat", "texture/skull_jawb.tga");
	jaw_back->attach_to(this);
	jaw_back->matrix = mat_scale(3.f);
	jaw_back->depth_offset = -1.f;
}

void Enemy::hit(const Damage_Data& data)
{
	fx->spawn_system<Enemy_Damage_System>(position, skull);
	fx->spawn_system<Enemy_Damage_System>(position, jaw_front);
	fx->spawn_system<Enemy_Damage_System>(position, jaw_back);

	scene->add_damage_number(position, data.damage, data.direction);

	velocity += data.impulse;

	health -= data.damage;
	if (health <= 0.f)
		scene->destroy_entity(this);
}

void Enemy::on_destroyed()
{
	scene->destroy_collider(collider);
	scene->destroy_drawable(skull);
	scene->destroy_drawable(jaw_back);
	scene->destroy_drawable(jaw_front);
}

void Enemy::update() 
{
	// Randomize offset to follow, to give them more of a "swarm" behavior
	offset_timer -= time_delta();
	if (offset_timer <= 0.f)
	{
		select_new_target_offset();
		offset_timer = Random::range(1.f, 4.f);
	}

	// Acceleration
	Vec3 target_point = game->player->position + target_offset;
	Vec3 direction = direction_to(position, target_point);

	velocity += direction * acceleration * time_delta();
	velocity -= velocity * friction * time_delta();

	// Sweeping against geometry
	Sweep_Info info;
	info.source_entity = this;
	info.ignore_self = true;
	info.object_mask = COBJ_World;

	Hit_Result hit = scene->sweep(Shape::sphere(position, 1.5f), velocity * time_delta(), info);
	if (hit.has_hit)
	{
		if (hit.is_penetrating)
			position += hit.normal * hit.penetration_depth;
		else
			velocity -= constrain_to_direction(velocity, hit.normal) * 1.6f;
	}

	// Move!
	position += velocity * time_delta();
	rotation = Quat::from_x(velocity);
	collider->position = position;

	// Update face twitching
	if (time_has_reached(next_mouth_twitch_time))
	{
		float openness = 1.f - (distance(position, game->player->position) / 20.f);
		openness = Math::saturate(openness);

		mouth_angle = Math::radians(Random::range(-5.f - 5.f * openness, 5.f + 5.f * openness) + 40.f * openness);
		next_mouth_twitch_time = time_elapsed() + 0.015f;
	}
	skull->matrix = Quat(Vec3::right, mouth_angle).matrix() * mat_scale(3.f);
	jaw_back->matrix = Quat(Vec3::right, -mouth_angle).matrix() * mat_scale(3.f);
	jaw_front->matrix = Quat(Vec3::right, -mouth_angle).matrix() * mat_scale(3.f);

	// Update player damage
	if (!game->player->is_immune() && game->player->is_alive())
	{
		if (distance_sqrd(position, game->player->position) < Math::square(1.5f))
			game->player->hit(direction_to(position, game->player->position));
	}
}

void Enemy::select_new_target_offset()
{
	target_offset = Vec3(Random::range(-4.f, 4.f), Random::range(-4.f, 4.f), 0.f);
}