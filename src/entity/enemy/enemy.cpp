#include "enemy.h"
#include "resource/resourcecommon.h"
#include "entity/player/player.h"
#include "game/game.h"
#include "game/scene.h"

void Enemy::init()
{
	collider = scene.add_collider();
	collider->set_aabb(Vec3(3.f));
	collider->owner = this;
	collider->position = position;
}

void Enemy::on_destroyed()
{
	scene.destroy_collider(collider);
}

void Enemy::update() 
{
	Vec3 direction = direction_to(position, game.player->position);
	position += direction * ENEMY_SPD * time_delta();

	collider->position = position;
}

void Enemy::render(const Render_Info& info)
{
	Common_Mat::test.use();
	Common_Mat::test.set("u_ViewProjection", info.view_projection);
	Common_Mat::test.set("u_Model", mat_translation(position) * mat_scale(3.f));

	Common_Mesh::rect.draw();
}
