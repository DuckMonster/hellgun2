#include "editor.h"
#include "core/input.h"
#include "core/context.h"
#include "collision/collision.h"

#include "scene.h"
#include "level.h"
#include "entity/entity.h"
#include "fx/fx.h"
#include "debug/debug.h"

#include "ui/ui.h"
#include "ui/wcanvas.h"
#include "ui/whorizontal_box.h"
#include "ui/wvertical_box.h"
#include "ui/wimage.h"
#include "ui/wbutton.h"
#include "ui/wtext.h"
#include "ui/wcollapse_tab.h"

#include "resource/resource.h"

#include "game.h"

Editor* editor;

void Editor::init()
{
	camera.fov = 90.f;
	camera.near = 0.1f;
	camera.far = 1000.f;

	sweep_src = Shape::aabb(Vec3::zero, Vec3(3.5f));
	sweep_tar = Shape::aabb(Vec3::zero, Vec3(2.5f));
}

void Editor::update()
{
	INPUT_SCOPE(Input_Group::Editor);

	// Update camera movement
	if (input->mouse_pressed(Mouse_Btn::Right))
		context.lock_cursor();
	if (input->mouse_released(Mouse_Btn::Right))
		context.unlock_cursor();

	if (input->mouse_down(Mouse_Btn::Right))
	{
		// Translation
		float delta = EDITOR_CAM_SPD * time_delta_raw();
		if (input->key_down(Key::W))
			camera.position += camera.forward() * delta;
		if (input->key_down(Key::S))
			camera.position -= camera.forward() * delta;
		if (input->key_down(Key::D))
			camera.position += camera.right() * delta;
		if (input->key_down(Key::A))
			camera.position -= camera.right() * delta;

		if (input->key_down(Key::E) || input->key_down(Key::Spacebar))
			camera.position += camera.up() * delta;
		if (input->key_down(Key::Q) || input->key_down(Key::LeftControl))
			camera.position -= camera.up() * delta;

		// Rotation
		float yaw_delta = -input->mouse_delta_x() * EDITOR_CAM_SENS;
		float pitch_delta = -input->mouse_delta_y() * EDITOR_CAM_SENS;

		Quat yaw_quat = Quat(Vec3(0.f, 1.f, 0.f), yaw_delta);
		Quat pitch_quat = Quat(Vec3(1.f, 0.f, 0.f), pitch_delta);

		camera.rotation = yaw_quat * camera.rotation * pitch_quat;
	}

	// Sweep testing
	if (input->key_down(Key::F))
	{
		has_sweep = true;
		sweep_origin = camera.position + camera.right();
		sweep_direction = camera.forward();

		if (input->key_down(Key::LeftShift))
			sweep_offset += input->mouse_wheel_delta() * 1.f;
		else
			sweep_length += input->mouse_wheel_delta() * 1.f;
	}

	if (input->key_pressed(Key::N))
		sweep_src.type = (Shape::Type)(((u32)sweep_src.type + 1) % (u32)Shape::Type::MAX);
	if (input->key_pressed(Key::M))
		sweep_tar.type = (Shape::Type)(((u32)sweep_tar.type + 1) % (u32)Shape::Type::MAX);

	if (has_sweep)
	{
		Vec3 start = sweep_origin + sweep_direction * sweep_offset;
		Vec3 delta = sweep_direction * sweep_length;

		sweep_src.position = start;

		Hit_Result hit = scene->sweep(sweep_src, delta, Sweep_Info());
		Color clr = hit.has_hit ? Color::red : Color::green;
		if (hit.is_penetrating)
			clr = Color::yellow;

		sweep_src.position = hit.position;

		debug->line(start, hit.position, clr);
		debug->line(hit.position, start + delta, Color::blue);
		if (hit.is_penetrating)
			debug->vector(hit.position, hit.normal * hit.penetration_depth, clr);
		else
			debug->vector(hit.position, hit.normal, clr);

		debug->print(String::printf("Time: %f", hit.time));
		sweep_src.debug_draw(clr);
	}

	// Editor UI
	Canvas_Style::anchor(Vec2(0.5f, 1.f));
	Canvas_Style::alignment(Vec2(0.5f, 1.f));
	Canvas_Style::position(Vec2(0.f, -50.f));

	auto* tab = ui->begin<WCollapse_Tab>();
	{
		ui->add<WText>("This is text.");
	}
	ui->end();
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
	for(auto* drawable : scene->drawables)
		drawable->render(info);

	fx->render(info);
	level->render(info);
}

void Editor::enter_editor()
{
	// Move camera to match game camera
	camera.position = game->camera.position;
	camera.rotation = game->camera.rotation;

	context.unlock_cursor();
}

void Editor::exit_editor()
{
	context.lock_cursor();
}