#include "editor.h"
#include "core/input.h"
#include "core/context.h"
#include "collision/collision.h"

#include "scene.h"
#include "level.h"
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

	sweep_src = Shape::aabb(Vec3::zero, Vec3(1.2f));
	sweep_tar = Shape::aabb(Vec3::zero, Vec3(2.5f));
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

	if (key_pressed(Key::N))
		sweep_src.type = (Shape::Type)(((u32)sweep_src.type + 1) % (u32)Shape::Type::MAX);
	if (key_pressed(Key::M))
		sweep_tar.type = (Shape::Type)(((u32)sweep_tar.type + 1) % (u32)Shape::Type::MAX);

	if (has_sweep)
	{
		sweep_src.position = sweep_origin;

		Hit_Result hit = sweep_src.sweep(sweep_direction * sweep_length, sweep_tar);
		Color clr = hit.has_hit ? Color::red : Color::green;

		sweep_src.position = hit.position;

		debug->line(sweep_origin, hit.position, clr);
		debug->line(hit.position, sweep_origin + sweep_direction * sweep_length, Color::blue);
		debug->vector(hit.position, hit.normal, clr);
		debug->print(String::printf("Time: %f", hit.time));
		sweep_src.debug_draw(clr);
		sweep_tar.debug_draw(clr);
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
	level->render(info);

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