#include "editor.h"
#include "core/input.h"
#include "core/context.h"
#include "collision/collision.h"

#include "scene.h"
#include "entity/entity.h"
#include "fx/fx.h"

#include "debug/debug.h"
#include "game.h"

Editor* editor;

void Editor::init()
{
	camera.fov = 90.f;
	camera.near = 0.1f;
	camera.far = 1000.f;
}

void Editor::update()
{
	// Time dilation
	if (key_pressed(Key::Plus))
		change_time_dilation(1);
	if (key_pressed(Key::Minus))
		change_time_dilation(-1);

	// Update camera movement
	if (mouse_pressed(Mouse_Btn::Right))
		context.lock_cursor();
	if (mouse_released(Mouse_Btn::Right))
		context.unlock_cursor();

	if (mouse_down(Mouse_Btn::Right))
	{
		// Translation
		float delta = EDITOR_CAM_SPD * time_delta_raw();
		if (key_down(Key::W))
			camera.position += camera.forward() * delta;
		if (key_down(Key::S))
			camera.position -= camera.forward() * delta;
		if (key_down(Key::D))
			camera.position += camera.right() * delta;
		if (key_down(Key::A))
			camera.position -= camera.right() * delta;

		if (key_down(Key::E) || key_down(Key::Spacebar))
			camera.position += camera.up() * delta;
		if (key_down(Key::Q) || key_down(Key::LeftControl))
			camera.position -= camera.up() * delta;

		// Rotation
		float yaw_delta = -mouse_delta_x() * EDITOR_CAM_SENS;
		float pitch_delta = -mouse_delta_y() * EDITOR_CAM_SENS;

		Quat yaw_quat = Quat(Vec3(0.f, 1.f, 0.f), yaw_delta);
		Quat pitch_quat = Quat(Vec3(1.f, 0.f, 0.f), pitch_delta);

		camera.rotation = yaw_quat * camera.rotation * pitch_quat;
	}

	// Sweep testing
	if (key_down(Key::F))
	{
		has_sweep = true;
		sweep_origin = camera.position + camera.right();
		sweep_direction = camera.forward();

		sweep_length += mouse_wheel_delta() * 1.f;
	}

	if (has_sweep)
	{
		// TESTING
		Capsule cap = Capsule(Vec3(5.f, 0.f, 0.f), Vec3(-5.f, 0.f, 0.f), 1.8f);
		Hit_Result hit = Collision::line_trace(sweep_origin, sweep_origin + sweep_direction * sweep_length, cap);

		Color clr = hit.has_hit ? Color::red : Color::green;
		debug->capsule(cap.a, cap.b, cap.radius, clr);
		debug->line(sweep_origin, hit.position, clr);
		debug->point(hit.position, clr);

		/*
		AABB box = AABB::from_center_size(Vec3::zero, Vec3(5.f));
		Hit_Result hit = Collision::line_trace(sweep_origin, sweep_origin + sweep_direction * sweep_length, box);

		Color clr = hit.has_hit ? Color::red : Color::green;
		if (hit.is_penetrating)
			clr = Color::yellow;

		debug->box(box.center(), box.size(), Quat::identity, clr);
		debug->line(sweep_origin, hit.position, clr);
		debug->point(hit.position, clr);

		if (hit.is_penetrating)
			debug->vector(hit.position, hit.normal * hit.penetration_depth, clr);
		else
			debug->vector(hit.position, hit.normal, clr);
			*/

/*
		Sphere a = Sphere(sweep_origin, 1.5f);
		AABB b = AABB::from_center_size(Vec3::zero, Vec3(5.f));
		Hit_Result hit = Collision::sweep_sphere(a, sweep_direction * sweep_length, b);

		Color clr = hit.has_hit ? Color::red : Color::blue;

		debug->sphere(a.origin, a.radius, clr);
		debug->line(a.origin, hit.position, clr);

		debug->sphere(hit.position, a.radius, clr);
		debug->box(b.center(), b.size(), Quat::identity, clr);

		debug->print(String::printf("Time: %f", hit.time), 0.f);

		if (hit.has_hit)
			debug->vector(hit.position, hit.normal, clr);

		debug->capsule(Vec3(-5.f, -2.f, 0.f), Vec3(-5.f, 2.f, 0.f), 1.f);
		*/

		/*
		AABB test_aabb = AABB::from_center_size(editor_camera.position + editor_camera.right(), Vec3::one);
		Hit_Result hit = scene->sweep_aabb(test_aabb, delta);

		Color clr = hit.has_hit ? Color::red : Color::blue;
		debug->vector(test_aabb.center(), delta, clr);
		debug->box(hit.position, test_aabb.size(), Quat::identity);

		if (hit.has_hit)
			debug->vector(hit.position, hit.normal * 2.f, clr);
			*/
	}
}

void Editor::render()
{
	Render_Info info;
	info.ui_canvas = mat_ortho(0.f, context.width, context.height, 0.f);

	info.view = camera.get_view();
	info.projection = camera.get_projection();
	info.view_projection = info.projection * info.view;

	for(auto* entity : scene->entities)
		entity->render(info);
	for(auto* collider : scene->colliders)
		collider->debug_draw(Color::blue);

	fx->render(info);
	debug->render(info);
}

void Editor::enter_editor()
{
	// Move camera to match game camera
	camera.position = game->camera.position;
	camera.rotation = game->camera.rotation;
}

void Editor::exit_editor()
{
	context.unlock_cursor();
}

void Editor::change_time_dilation(i32 delta)
{
	time_dilation_exp += delta;
	time_dilation_exp = Math::clamp(time_dilation_exp, -8, 8);
	time_dilate(Math::pow(1.5f, time_dilation_exp));

	debug->print(String::printf("time dilation: %f\n", Math::pow(1.5f, time_dilation_exp)), 3.f);
}