#include "player.h"
#include "core/input.h"
#include "debug/debug.h"
#include "game/game.h"
#include "game/scene.h"
#include "resource/resource_common.h"
#include "math/plane.h"
#include "resource/resource.h"
#include "fx/fx.h"
#include "fx/weapon/muzzle_flash_system.h"
#include "fx/surface_impact_system.h"
#include "weapon/pistol/pistol.h"
#include "weapon/cross/cross.h"

#include <stdio.h>

void Player::init()
{
	weapons.add(new Pistol());
	weapons.add(new Cross());

	// Load crosshair meshes
	crosshair_circle.init();
	crosshair_circle.add_buffer(0);
	crosshair_circle.bind_attribute(0, 0, 2, 2 * sizeof(float), 0);

	constexpr u32 CIRCLE_RESOLUTION = 32;
	Vec2 circle_data[CIRCLE_RESOLUTION];
	float angle_step = TAU / CIRCLE_RESOLUTION;

	for(u32 i = 0; i < CIRCLE_RESOLUTION; ++i)
		circle_data[i] = Vec2(Math::cos(angle_step * i), Math::sin(angle_step * i));

	crosshair_circle.buffer_data(0, sizeof(circle_data), circle_data);
	crosshair_circle.draw_num = CIRCLE_RESOLUTION;
	crosshair_circle.draw_mode = GL_LINE_LOOP;

	crosshair_line.init();
	crosshair_line.add_buffer(0);
	crosshair_line.bind_attribute(0, 0, 2, 2 * sizeof(float), 0);
	crosshair_line.buffer_data(0, sizeof(Vec2) * 2, circle_data); // just temp data

	float line_alphas[] = { 0.f, 1.f };
	crosshair_line.add_buffer(1);
	crosshair_line.bind_attribute(1, 1, 1, 1 * sizeof(float), 0);
	crosshair_line.buffer_data(1, sizeof(line_alphas), line_alphas);

	crosshair_line.draw_num = 2;
	crosshair_line.draw_mode = GL_LINES;
}

void Player::update()
{
	if (!is_alive())
		return;

	Vec3 input = Vec3::zero;
	if (key_down(Key::A))
		input.x -= 1.f;
	if (key_down(Key::D))
		input.x += 1.f;

	if (key_pressed(Key::Q))
	{
		weapons[equipped_weapon]->on_unequipped();
		equipped_weapon = (equipped_weapon + 1) % weapons.count();
		weapons[equipped_weapon]->on_equipped();
	}

	weapons[equipped_weapon]->update();

	update_movement();
	move(velocity * time_delta());

	debug->print(String::printf("HEALTH: %d", health));
}

void Player::update_movement()
{
	// Update grounded
	if (is_grounded)
	{
		Hit_Result hit = sweep(Vec3(0.f, -0.1f, 0.f));

		// No ground under us, we're airborne!
		if (!hit.has_hit)
			is_grounded = false;
	}
	else
	{
		// Only re-ground ourselves if we're actully moving downwards
		if (velocity.y < 1.2f)
		{
			Hit_Result hit = sweep(Vec3(0.f, -0.1f, 0.f));

			if (hit.has_hit)
			{
				position = hit.position;
				is_grounded = true;
			}
		}
	}

	if (is_grounded)
		update_grounded();
	else
		update_airborne();

	// Update friction scale
	friction_scale = Math::lerp(friction_scale, 1.f, 2.5f * time_delta());

	// Dashing
	if (key_pressed(Key::S) && !is_grounded)
	{
		velocity = Vec3(0.f, -1.f, 0.f) * PLAYER_GROUND_POUND_IMPULSE;
		friction_scale = 0.f;

		// Spawn jump FX
		fx->spawn_system<Surface_Impact_System>(
			position + Vec3(0.f, 0.5f, 0.f),
			normalize(velocity),
			Vec3::zero,
			-normalize(velocity),
			0.5f
		);
	}
	if (key_pressed(Key::LeftShift) && get_movement_input() != 0.f)
	{
		velocity = Vec3(1.f, 0.f, 0.f) * get_movement_input() * PLAYER_GROUND_DASH_IMPULSE;
		friction_scale = 0.f;

		// Spawn jump FX
		fx->spawn_system<Surface_Impact_System>(
			position + Vec3(0.f, -0.5f, 0.f),
			normalize(velocity),
			Vec3::zero,
			-normalize(velocity),
			0.5f
		);
	}
}

void Player::update_grounded()
{
	// Move input
	velocity.x += get_movement_input() * PLAYER_GROUND_ACC * friction_scale * time_delta();

	// Friction
	velocity -= velocity * PLAYER_GROUND_FRICTION * friction_scale * time_delta();

	// Jumping
	air_jumps = 1;
	if (key_pressed(Key::Spacebar))
	{
		velocity.y = PLAYER_JMP_IMPULSE;

		// Spawn jump FX
		fx->spawn_system<Surface_Impact_System>(
			position + Vec3(0.f, -0.5f, 0.f),
			Vec3::up,
			Vec3::zero,
			normalize(velocity),
			0.5f
		);
	}
}

void Player::update_airborne()
{
	// Move input
	velocity.x += get_movement_input() * PLAYER_AIR_ACC * friction_scale * time_delta();

	// Gravity
	velocity -= Vec3::up * PLAYER_GRAVITY * friction_scale * time_delta();

	// Friction
	velocity.x -= velocity.x * PLAYER_AIR_FRICTION_H * friction_scale * time_delta();
	velocity.y -= velocity.y * PLAYER_AIR_FRICTION_V * friction_scale * time_delta();

	if (key_pressed(Key::Spacebar) && air_jumps > 0)
	{
		velocity.y = PLAYER_AIR_JMP_IMPULSE;
		air_jumps--;

		// Spawn jump FX
		fx->spawn_system<Surface_Impact_System>(
			position + Vec3(0.f, -0.5f, 0.f),
			normalize(velocity),
			Vec3::zero,
			-normalize(velocity),
			0.5f
		);
	}
}

void Player::move(Vec3 delta)
{
	int it = 0;
	while(!is_nearly_zero(delta) && ++it < 10)
	{
		Hit_Result hit = sweep(delta);
		if (hit.is_penetrating)
		{
			position += hit.normal * hit.penetration_depth;
			continue;
		}

		if (hit.has_hit)
		{
			position = hit.position;
			velocity = constrain_to_plane(velocity, hit.normal);
			delta = constrain_to_plane(delta, hit.normal);

			delta = delta * (1.f - hit.time);
			continue;
		}

		position += delta;
		return;
	}
}

Hit_Result Player::sweep(const Vec3& delta)
{
	Shape shape = Shape::aabb(position, Vec3::one);

	Sweep_Info info;
	info.source_entity = this;
	info.ignore_self = true;
	info.object_mask = COBJ_World;

	return scene->sweep(shape, delta, info);
}

float Player::get_movement_input()
{
	float input = 0.f;
	if (key_down(Key::D))
		input += 1.f;
	if (key_down(Key::A))
		input -= 1.f;

	return input;
}

void Player::hit(const Vec3& direction)
{
	health -= 1;
	velocity *= 0.4f;
	velocity += direction * PLAYER_HIT_IMPULSE;

	immune_until = time_elapsed() + PLAYER_IMMUNE_TIME;
}

void Player::render(const Render_Info& info)
{
	if (!is_alive())
		return;

	// Blink while immune :)
	if (is_immune() && int(time_elapsed_raw() * 10.f) % 2)
		return;

	Material* mat = Resource::load_material("material/default.mat");
	mat->use();
	mat->set("u_ViewProjection", info.view_projection);
	mat->set("u_Model", mat_translation(position));

	Mesh* mesh = Resource::load_mesh("mesh/plane.msh");
	mesh->draw();

	// Render equipped weapon
	weapons[equipped_weapon]->render(info);
}

void Player::render_ui(const Render_Info& info)
{
	glEnable(GL_BLEND);

	// Draw circle
	Material* mat = Resource::load_material("material/ui/crosshair.mat");
	mat->use();
	mat->set("u_ViewProjection", info.ui_canvas);
	mat->set("u_Model", mat_translation(Vec3(mouse_x(), mouse_y(), 0.f)) * mat_scale(Vec3(4.f)));

	crosshair_circle.draw();

	// Draw line
	mat = Resource::load_material("material/ui/crosshair_line.mat");
	mat->use();
	mat->set("u_ViewProjection", info.ui_canvas);

	Vec2 line_start = info.world_to_canvas(position);
	Vec2 line_end = Vec2(mouse_x(), mouse_y());

	// Offset line_end to be at the boundary of the circle
	Vec2 dir = normalize(line_end - line_start);
	line_end -= dir * 4.f;

	// Offset line_start to be at the middle-point
	line_start = Math::lerp(line_start, line_end, 0.5f);

	Vec2 line_verts[2];
	line_verts[0] = line_start;
	line_verts[1] = line_end;

	crosshair_line.buffer_subdata(0, 0, sizeof(line_verts), line_verts);
	crosshair_line.draw();

	glDisable(GL_BLEND);
}