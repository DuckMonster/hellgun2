#include "enemy.h"
#include "resource/resourcecommon.h"
#include "resource/resource.h"
#include "player/player.h"
#include "game/game.h"
#include "game/scene.h"
#include "fx/fx.h"
#include "fx/trailsystem.h"
#include "math/random.h"

void Enemy::init()
{
	acceleration = Random::range(ENEMY_ACC - 5.f, ENEMY_ACC + 5.f);
	friction = Random::range(ENEMY_FRICTION - 0.2f, ENEMY_FRICTION + 0.2f);

	collider = scene->add_collider();
	collider->object_type = COBJ_Enemy;
	collider->set_aabb(Vec3(3.f));
	collider->owner = this;
	collider->position = position;

	Trail_Params params;
	params.color = Color::red;
	params.duration = 2.f;

	trail = fx->spawn_system<Trail_System>(position, params);
	trail->attach_to(this);
}

void Enemy::on_destroyed()
{
	scene->destroy_collider(collider);

	trail->finish_system();
	trail->detach();
}

void Enemy::update() 
{
	offset_timer -= time_delta();
	if (offset_timer <= 0.f)
	{
		select_new_target_offset();
		offset_timer = Random::range(1.f, 4.f);
	}

	Vec3 target_point = game->player->position + target_offset;
	Vec3 direction = direction_to(position, target_point);

	velocity += direction * acceleration * time_delta();
	velocity -= velocity * friction * time_delta();

	Sweep_Info info;
	info.source_entity = this;
	info.ignore_self = true;
	info.object_mask = COBJ_World;

	Hit_Result hit = scene->sweep_aabb(AABB::from_center_size(position, Vec3(3.f)), velocity * time_delta(), info);
	if (hit.has_hit)
		velocity -= constrain_to_direction(velocity, hit.normal) * 1.6f;

	position += velocity * time_delta();

	collider->position = position;
}

void Enemy::render(const Render_Info& info)
{
	Material* mat = Resource::load_material("material/test.mat");

	mat->use();
	mat->set("u_ViewProjection", info.view_projection);
	mat->set("u_Model", mat_translation(position) * mat_scale(3.f));

	Common_Mesh::rect.draw();
}

void Enemy::select_new_target_offset()
{
	target_offset = Vec3(Random::range(-4.f, 4.f), Random::range(-4.f, 4.f), 0.f);
}