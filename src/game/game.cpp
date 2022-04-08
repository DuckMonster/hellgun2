#include "game.h"
#include "debug/debug.h"
#include "core/input.h"
#include "core/context.h"
#include "collision/collision.h"
#include "collision/collider.h"
#include "scene.h"

#include "entity/player/player.h"
#include "entity/enemy/enemy.h"

#include "math/random.h"

#include "fx/emitter.h"

#include <stdio.h>

Game game;
Emitter emitter;

void Game::init()
{
	camera.rotation = Quat::identity;
	camera.position = Vec3(0.f, 0.f, 40.f);
	camera.fov = 45.f;
	camera.near = 1.f;
	camera.far = 50.f;

	editor_camera.rotation = Quat::identity;
	editor_camera.fov = 90.f;
	editor_camera.near = 0.1f;
	editor_camera.far = 1000.f;

	// Spawn player
	player = scene.spawn_entity<Player>(Vec3::zero);

	// Create "map"
	Collider* collider;

	// Bottom wall
	collider = scene.add_collider();
	collider->position = Vec3(0.f, -25.f, 0.f);
	collider->set_aabb(Vec3(50.f, 1.f, 10.f));

	// Top wall
	collider = scene.add_collider();
	collider->position = Vec3(0.f, 25.f, 0.f);
	collider->set_aabb(Vec3(50.f, 1.f, 10.f));

	// Left wall
	collider = scene.add_collider();
	collider->position = Vec3(-25.f, 0.f, 0.f);
	collider->set_aabb(Vec3(1.f, 50.f, 10.f));

	// Right wall
	collider = scene.add_collider();
	collider->position = Vec3(25.f, 0.f, 0.f);
	collider->set_aabb(Vec3(1.f, 50.f, 10.f));

	// Enemy spawning stuff
	enemy_spawn_time = 2.f;

	emitter.init();
	emitter.position = Vec3(0.f, -25.f, 0.f);
}

void Game::update()
{
	// Time dilation
	if (key_pressed(Key::Plus))
		change_time_dilation(1);
	if (key_pressed(Key::Minus))
		change_time_dilation(-1);

	// Editor mode!
	if (key_pressed(Key::Tab))
	{
		editor_mode = !editor_mode;
		editor_camera.position = camera.position;
		editor_camera.rotation = camera.rotation;
	}

	// Update camera
	{
		Vec3 target_position = Math::lerp(player->position, player->calculate_aim_position(), 0.3f);
		camera.position = Math::lerp(camera.position, target_position, 15.f * time_delta());
		camera.position.z = 40.f;
	}

	// Update enemy spawning
	enemy_spawn_time -= time_delta();
	if (enemy_spawn_time <= 0.f)
	{
		Vec3 spawn_pos = Vec3(Random::range(-10.f, 10.f), Random::range(0.f, 10.f), 0.f);
		//scene.spawn_entity<Enemy>(spawn_pos);

		enemy_spawn_time = Random::range(1.5f, 2.5f);
	}

	if (editor_mode)
	{
		editor_update();
	}
	else
	{
		for(auto* entity : scene.entities)
			entity->update();
	}

	if (scene.pending_destruction)
		scene.finish_destruction();

	emitter.update();

	// Debug text
	debug.text(String::printf("FPS: %d", (u32)(1.f / time_delta())), Vec2(context.width, 0.f), Color::yellow, Color::yellow * 0.4f, Vec2(1.f, 0.f));
	debug.text(String::printf("Entity count: %d", scene.entities.count()), Vec2(context.width, 12.f), Color::yellow, Color::yellow * 0.4f, Vec2(1.f, 0.f));
}

void Game::editor_update()
{
	if (mouse_pressed(Mouse_Btn::Right))
		context.lock_cursor();
	if (mouse_released(Mouse_Btn::Right))
		context.unlock_cursor();

	// Camera movement
	if (mouse_down(Mouse_Btn::Right))
	{
		// Translation
		float delta = EDITOR_CAM_SPD * time_delta_raw();
		if (key_down(Key::W))
			editor_camera.position += editor_camera.forward() * delta;
		if (key_down(Key::S))
			editor_camera.position -= editor_camera.forward() * delta;
		if (key_down(Key::D))
			editor_camera.position += editor_camera.right() * delta;
		if (key_down(Key::A))
			editor_camera.position -= editor_camera.right() * delta;

		if (key_down(Key::E) || key_down(Key::Spacebar))
			editor_camera.position += editor_camera.up() * delta;
		if (key_down(Key::Q) || key_down(Key::LeftControl))
			editor_camera.position -= editor_camera.up() * delta;

		// Rotation
		float yaw_delta = -mouse_delta_x() * EDITOR_CAM_SENS;
		float pitch_delta = -mouse_delta_y() * EDITOR_CAM_SENS;

		Quat yaw_quat = Quat(Vec3(0.f, 1.f, 0.f), yaw_delta);
		Quat pitch_quat = Quat(Vec3(1.f, 0.f, 0.f), pitch_delta);

		editor_camera.rotation = yaw_quat * editor_camera.rotation * pitch_quat;
	}

	// Sweep testing
	{
		AABB test_aabb = AABB::from_center_size(editor_camera.position + editor_camera.right(), Vec3::one);
		Vec3 delta = editor_camera.forward() * 50.f;
		Hit_Result hit = scene.sweep_aabb(test_aabb, delta);

		Color clr = hit.has_hit ? Color::red : Color::blue;
		debug.vector(test_aabb.center(), delta, clr);
		debug.box(hit.position, test_aabb.size(), Quat::identity);

		if (hit.has_hit)
			debug.vector(hit.position, hit.normal * 2.f, clr);
	}
}

void Game::render()
{
	Render_Info info;
	info.ui_canvas = mat_ortho(0.f, context.width, context.height, 0.f);

	if (editor_mode)
	{
		info.view = editor_camera.get_view();
		info.projection = editor_camera.get_projection();
	}
	else
	{
		info.view = camera.get_view();
		info.projection = camera.get_projection();
	}
	info.view_projection = info.projection * info.view;

	for(auto* entity : scene.entities)
		entity->render(info);
	for(auto* collider : scene.colliders)
		collider->debug_draw(Color::blue);

	emitter.render(info);

	debug.render(info);
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

void Game::change_time_dilation(i32 delta)
{
	time_dilation_exp += delta;
	time_dilation_exp = Math::clamp(time_dilation_exp, -8, 8);
	time_dilate(Math::pow(1.5f, time_dilation_exp));

	printf("time dilation: %f\n", Math::pow(1.5f, time_dilation_exp));
}