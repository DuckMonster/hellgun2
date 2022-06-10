#include "enemy.h"
#include "resource/resource_common.h"
#include "resource/resource.h"
#include "player/player.h"
#include "game/game.h"
#include "game/scene.h"
#include "debug/debug.h"
#include "fx/fx.h"
#include "fx/enemy/enemy_damage_system.h"
#include "fx/weapon/impact_spike_system.h"
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
}

void Enemy::hit(const Damage_Data& data)
{
	Material* mat = Resource::load_material("material/sprite.mat");
	Mesh* mesh = Resource::load_mesh("mesh/plane.msh");
	Texture* skull = Resource::load_texture("texture/skull.tga");
	Mat4 orient = mat_orient_x(normalize(velocity));

	fx->spawn_system<Enemy_Damage_System>(position, mesh, skull, mat_translation(position) * orient * mat_scale(3.f));
	fx->spawn_system<Impact_Spike_System>(position, data.visual_scale, 0.15f, data.direction);

	scene->add_damage_number(position, data.damage, data.direction);

	velocity += data.impulse;
}

void Enemy::on_destroyed()
{
	Material* mat = Resource::load_material("material/sprite.mat");
	Mesh* mesh = Resource::load_mesh("mesh/plane.msh");
	Texture* skull = Resource::load_texture("texture/skull.tga");
	Mat4 orient = mat_orient_x(normalize(velocity));

	fx->spawn_system<Enemy_Damage_System>(position, mesh, skull, mat_translation(position) * orient * mat_scale(3.f));
	scene->destroy_collider(collider);
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
	collider->position = position;

	// Update face twitching
	if (time_has_reached(next_mouth_twitch_time))
	{
		float openness = 1.f - (distance(position, game->player->position) / 20.f);
		openness = Math::saturate(openness);

		mouth_angle = Math::radians(Random::range(-5.f - 5.f * openness, 5.f + 5.f * openness) + 40.f * openness);
		next_mouth_twitch_time = time_elapsed() + 0.015f;
	}
}

void Enemy::render(const Render_Info& info)
{
	Material* mat = Resource::load_material("material/sprite.mat");
	Mesh* mesh = Resource::load_mesh("mesh/plane.msh");
	Texture* skull = Resource::load_texture("texture/skull.tga");
	Texture* jaw_front = Resource::load_texture("texture/skull_jawf.tga");
	Texture* jaw_back = Resource::load_texture("texture/skull_jawb.tga");

	// Main orientation of the skull
	Mat4 orient = mat_orient_x(normalize(velocity));

	mat->use();
	mat->set("u_ViewProjection", info.view_projection);
	glEnable(GL_POLYGON_OFFSET_FILL); // Used to do depth offset

	// Draw main skull, rotated counter-clockwise
	glPolygonOffset(0.f, 0.f);
	mat->set("u_Model", mat_translation(position) * orient * Quat(Vec3::right, mouth_angle).matrix() * mat_scale(3.f));
	skull->bind();
	mesh->draw();

	// Draw jaw, rotated clockwise
	mat->set("u_Model",
		mat_translation(position) *
		orient *
		Quat(Vec3::right, -mouth_angle).matrix() *
		mat_scale(3.f)
	);

	// Front jaw, offset depth to further forward
	glPolygonOffset(0.f, -0.1f);
	jaw_front->bind();
	mesh->draw();

	// Back jaw, offset depth to further behind
	glPolygonOffset(0.f, 0.1f);
	jaw_back->bind();
	mesh->draw();
}

void Enemy::select_new_target_offset()
{
	target_offset = Vec3(Random::range(-4.f, 4.f), Random::range(-4.f, 4.f), 0.f);
}