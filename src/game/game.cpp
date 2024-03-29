#include "game.h"
#include "core/input.h"
#include "core/context.h"
#include "collision/collision.h"
#include "collision/collider.h"
#include "scene.h"
#include "level.h"
#include "item_shop.h"

#include "resource/resource.h"

#include "player/player.h"
#include "player/weapon/weapon.h"
#include "player/item_bank.h"
#include "player/player_inventory.h"
#include "entity/enemy/enemy.h"

#include "math/random.h"
#include "math/plane.h"
#include "fx/fx.h"

Game* game;

void Game::init()
{
	// Load level
	level = Resource::load_level("level/test.lvl");
	level->open();

	// Init game systems
	item_shop = new Item_Shop();
	inventory = new Player_Inventory();
	inventory->init();

	register_all_items();

	context.lock_cursor();

	restart();
}

void Game::restart()
{
	// Destroy all entities
	for(auto* entity : scene->entities)
		scene->destroy_entity(entity);
	scene->finish_destruction();

	// Reset camera
	camera.rotation = Quat::identity;
	camera.position = Vec3(0.f, 0.f, 40.f);
	camera.fov = 45.f;
	camera.near = 1.f;
	camera.far = 100.f;

	// Spawn player
	player = scene->spawn_entity<Player>(Vec3::zero);

	// Load level
	level = Resource::load_level("level/test.lvl");
	level->open();

	next_spawn_time = 0.f;
}

void Game::update()
{
	// DEV item shop
	if (input->key_pressed(Key::B))
	{
		if (item_shop->is_open)
			item_shop->close();
		else
			item_shop->open();
	}

	if (item_shop->is_open)
		item_shop->update_ui();

	INPUT_SCOPE(Input_Group::Game);

	// Update camera
	{
		Vec3 target_position = player->position;
		if (input->is_group_active(Input_Group::Game))
			target_position += (get_mouse_game_position() - player->position) * 0.3f;

		camera.position = Math::lerp(camera.position, target_position, 15.f * time_delta());
		camera.position.z = 55.f;
	}

	// Restart
	if (input->key_pressed(Key::R))
	{
		restart();
		return;
	}

	// Update enemy spawning
	if (input->key_pressed(Key::E))
	{
		spawn_enemies = !spawn_enemies;
	}

	if (spawn_enemies && time_has_reached(next_spawn_time))
	{
		Vec3 spawn_pos = Vec3(Random::range(-15.f, 15.f), Random::range(-15.f, 15.f), 0.f);
		for(u32 i = 0; i < 8; i++)
		{
			Vec3 enemy_pos = spawn_pos + Random::point_on_circle(Vec3::right) * Random::range(0.f, 1.5f);

			Enemy* enemy = scene->spawn_entity<Enemy>(spawn_pos);
			enemy->velocity = Random::point_on_circle(Vec3::right) * Random::range(5.f, 15.f);
		}

		next_spawn_time = time_elapsed() + 10.f;
	}

	for(auto* entity : scene->entities)
		entity->update();

	for(auto& damage_nmbr : scene->damage_numbers)
	{
		if (damage_nmbr.active)
			damage_nmbr.update();
	}

	fx->update();

	if (scene->pending_destruction)
		scene->finish_destruction();

	player->update_ui();
}

void Game::render()
{
	Render_Info info;
	info.ui_canvas = mat_ortho(0.f, context.width, context.height, 0.f);

	info.view = camera.get_view();
	info.projection = camera.get_projection();
	info.view_projection = info.projection * info.view;

	for(auto* entity : scene->entities)
		entity->render(info);

	for(auto* drawable : scene->drawables)
		drawable->render(info);

	for(auto& damage_nmbr : scene->damage_numbers)
	{
		if (damage_nmbr.active)
			damage_nmbr.render(info);
	}

	level->render(info);
	fx->render(info);
}

Ray Game::get_mouse_world_ray()
{
	Mat4 projview_inv = inverse(camera.get_view_projection());

	Vec4 ndc = Vec4(
		input->mouse_x() / context.width,
		input->mouse_y() / context.height,
		-1.f,
		1.f
	);

	// Convert [0, 1] range to [-1, 1]
	// Also flip the y-axis
	ndc.x = ndc.x * 2.f - 1.f;
	ndc.y = ndc.y * -2.f + 1.f;

	// Deproject near
	Vec4 near = projview_inv * ndc;
	near = near / near.w;

	// Deproject far
	ndc.z = 1.f;
	Vec4 far = projview_inv * ndc;
	far = far / far.w;

	Vec3 origin = Vec3(near.x, near.y, near.z);
	Vec3 direction = normalize(Vec3(far.x, far.y, far.z) - origin);

	return Ray(origin, direction);
}

Vec3 Game::get_mouse_game_position()
{
	Plane game_plane = Plane(Vec3::zero, Vec3(0.f, 0.f, 1.f));
	Ray ray = get_mouse_world_ray();

	Vec3 position;
	ray.intersect(game_plane, &position);

	return position;
}