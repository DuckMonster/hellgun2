#include "enemy.h"
#include "resource/resourcecommon.h"
#include "resource/resource.h"
#include "player/player.h"
#include "game/game.h"
#include "game/scene.h"
#include "debug/debug.h"
#include "fx/fx.h"
#include "fx/enemy/enemydamagesystem.h"
#include "gfx/texture.h"
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

	jaw_location = position;
}

void Enemy::on_destroyed()
{
	fx->spawn_system<Enemy_Damage_System>(position, &Common_Mesh::rect, mat_translation(position) * mat_scale(3.f));
	scene->destroy_collider(collider);
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

	Hit_Result hit = scene->sweep(Shape::aabb(position, Vec3(3.f)), velocity * time_delta(), info);
	if (hit.has_hit)
		velocity -= constrain_to_direction(velocity, hit.normal) * 1.6f;

	position += velocity * time_delta();


	jaw_location = Math::lerp(jaw_location, position, 25.f * time_delta());

	collider->position = position;
}

void Enemy::render(const Render_Info& info)
{
	Material* mat = Resource::load_material("material/sprite.mat");
	Mesh* mesh = Resource::load_mesh("mesh/plane.msh");
	Texture* skull = Resource::load_texture("texture/skull.tga");
	Texture* jaw_front = Resource::load_texture("texture/skull_jawf.tga");
	Texture* jaw_back = Resource::load_texture("texture/skull_jawb.tga");

	float angle = vec_to_angle((Vec2)velocity);

	mat->use();
	mat->set("u_ViewProjection", info.view_projection);
	mat->set("u_Model", mat_translation(position) * Quat(Vec3::right, angle).matrix() * mat_scale(3.f));

	// Draw front jaw
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.f, 0.f);
	skull->bind();
	mesh->draw();

	mat->set("u_Model", mat_translation(jaw_location) * Quat(Vec3::right, angle).matrix() * mat_scale(3.f));
	glPolygonOffset(0.f, -0.1f);
	jaw_front->bind();
	mesh->draw();

	glPolygonOffset(0.f, 0.1f);
	jaw_back->bind();
	mesh->draw();

	debug->print(String::printf("angle: %f", vec_to_angle((Vec2)velocity)));
}

void Enemy::select_new_target_offset()
{
	target_offset = Vec3(Random::range(-4.f, 4.f), Random::range(-4.f, 4.f), 0.f);
}