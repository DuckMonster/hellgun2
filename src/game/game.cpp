#include "game.h"
#include "debug/debug.h"
#include "core/input.h"
#include "core/context.h"
#include "collision/collision.h"
#include "collision/collider.h"
#include "math/matrix_math.h"
#include "scene.h"
#include <stdio.h>

Game game;

void Game::init()
{
	camera.rotation = Quat::identity;
	camera.position = Vec3(0.f, 0.f, 40.f);
	camera.fov = 45.f;
	camera.near_plane = 1.f;
	camera.far_plane = 50.f;

	editor_camera.rotation = Quat::identity;
	editor_camera.fov = 90.f;
	editor_camera.near_plane = 0.1f;
	editor_camera.far_plane = 1000.f;

	player.init();

	Collider* collider = scene.add_collider();
	collider->position = Vec3(0.f, -5.f, 0.f);
	collider->set_aabb(Vec3(10.f, 1.f, 10.f));
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

	if (editor_mode)
	{
		editor_update();
	}
	else
	{
		player.update();
	}
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
		Collider a;
		a.set_aabb(Vec3(1.f));
		a.position = editor_camera.position + editor_camera.right() * 1.f;

		Collider b;
		b.set_aabb(Vec3(5.f, 10.f, 20.f));
		b.position = Vec3(-5.f, 0.f, 0.f);

		Hit_Result hit = a.sweep(editor_camera.forward() * 50.f, &b);

		Color clr = Color::blue;
		if (hit.has_hit)
			clr = Color::red;

		a.debug_draw(clr);
		b.debug_draw(clr);

		if (hit.is_penetrating)
		{
			debug.vector(hit.location, hit.normal * hit.penetration_depth, Color::red);
			debug.box(hit.location + hit.normal * hit.penetration_depth, a.size, Quat::identity, Color::red);
		}
		else
		{
			if (hit.has_hit)
				debug.vector(hit.location, hit.normal * 2.f, clr);

			debug.line(a.position, hit.location, clr);
			debug.box(hit.location, a.size, Quat::identity, clr);
		}
	}
}

void Game::render()
{
	Render_Info info;

	if (editor_mode)
	{
		info.view = editor_camera.get_view();
		info.projection = editor_camera.get_projection();

		for(const auto& collider : scene.colliders)
			collider.debug_draw(Color::blue);
	}
	else
	{
		info.view = camera.get_view();
		info.projection = camera.get_projection();
	}
	info.view_projection = info.projection * info.view;

	player.render(info);
	debug.render(info);
}

void Game::change_time_dilation(i32 delta)
{
	time_dilation_exp += delta;
	time_dilation_exp = Math::clamp(time_dilation_exp, -8, 8);
	time_dilate(Math::pow(1.5f, time_dilation_exp));

	printf("time dilation: %f\n", Math::pow(1.5f, time_dilation_exp));
}