#include "game.h"
#include "debug/debug.h"
#include "core/input.h"
#include "core/context.h"
#include "collision/collision.h"
#include "collision/collider.h"
#include "scene.h"
#include "level.h"

#include "resource/resource.h"

#include "player/player.h"
#include "entity/enemy/enemy.h"

#include "math/random.h"
#include "math/plane.h"
#include "fx/fx.h"
#include "ui/ui.h"

Game* game;

void Game::init()
{
	camera.rotation = Quat::identity;
	camera.position = Vec3(0.f, 0.f, 40.f);
	camera.fov = 45.f;
	camera.near = 1.f;
	camera.far = 100.f;

	editor_camera.rotation = Quat::identity;
	editor_camera.fov = 90.f;
	editor_camera.near = 0.1f;
	editor_camera.far = 1000.f;

	// Spawn player
	player = scene->spawn_entity<Player>(Vec3::zero);

	// Enemy spawning stuff
	enemy_spawn_time = 2.f;

	// Load level
	level = Resource::load_level("level/test.lvl");
	level->open();
}

void Game::update()
{
	// Update camera
	{
		Vec3 target_position = Math::lerp(player->position, player->calculate_aim_position(), 0.3f);
		camera.position = Math::lerp(camera.position, target_position, 15.f * time_delta());
		camera.position.z = 55.f;
	}

	// Update enemy spawning
	if (key_pressed(Key::E))
	{
		Vec3 spawn_pos = Vec3(Random::range(-10.f, 10.f), Random::range(0.f, 10.f), 0.f);
		scene->spawn_entity<Enemy>(spawn_pos);
	}

	for(auto* entity : scene->entities)
		entity->update();

	fx->update();

	if (scene->pending_destruction)
		scene->finish_destruction();
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

	fx->render(info);
	level->render(info);

	debug->render(info);
	ui->render(info);
}

Ray Game::get_mouse_world_ray()
{
	Mat4 projview_inv = inverse(camera.get_view_projection());

	Vec4 ndc = Vec4(
		mouse_x() / context.width,
		mouse_y() / context.height,
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
