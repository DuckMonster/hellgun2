#include "player.h"
#include "core/input.h"
#include "debug/debug.h"
#include "game/game.h"
#include "game/scene.h"
#include "resource/resource.h"
#include "fx/fx.h"
#include "fx/surface_impact_system.h"
#include "weapon/weapon.h"
#include "crosshair.h"

#include "ui/ui.h"
#include "ui/container/wcanvas.h"
#include "ui/container/wbox.h"
#include "ui/container/wvertical_box.h"
#include "ui/container/whorizontal_box.h"
#include "ui/visual/wimage.h"

#include "player_inventory.h"

#include <stdio.h>

static Key weapon_equip_keys[] = 
{
	Key::Num1,
	Key::Num2,
	Key::Num3,
	Key::Num4,
};

void Player::init()
{
	// Meshes
	mesh = scene->add_drawable();
	mesh->load("mesh/plane.msh", "material/default.mat");
	mesh->attach_to(this);

	// Load crosshair meshes
	// Circle
	crosshair_circle = scene->add_drawable();
	crosshair_circle->material = Resource::load_material("material/ui/crosshair.mat");
	crosshair_circle->mesh = get_crosshair_circle();
	crosshair_circle->flags = DRW_UI | DRW_Blend;

	crosshair_circle->on_render.bind_lambda([this](const Render_Info& info)
	{
		crosshair_circle->matrix = mat_translation(Vec3(input->mouse_x(), input->mouse_y(), 0.f)) * mat_scale(4.f);
	});

	// Line
	Vec2 line_data[] = { Vec2(0.5f, 0.f), Vec2(1.f, 0.f) };
	crosshair_line = scene->add_drawable();
	crosshair_line->material = Resource::load_material("material/ui/crosshair_line.mat");
	crosshair_line->mesh = get_crosshair_line();
	crosshair_line->flags = DRW_UI | DRW_Blend;

	crosshair_line->on_render.bind_lambda([this](const Render_Info& info)
	{
		Vec2 src = info.world_to_canvas(position);
		Vec2 tar = input->mouse_position();

		float length = ::length(tar - src);
		Vec2 dir = (tar - src) / length;

		crosshair_line->matrix = Mat4(
			Vec3(dir * (length - 4.f)),
			Vec3::zero,
			Vec3::zero,
			Vec3(src)
		);
	});
}

void Player::on_destroyed()
{
	scene->destroy_drawable(mesh);
	scene->destroy_drawable(crosshair_line);
	scene->destroy_drawable(crosshair_circle);
}

void Player::equip_weapon(Weapon* weapon)
{
	if (equipped_weapon)
		equipped_weapon->on_unequipped();
	if (weapon)
		weapon->on_equipped();

	equipped_weapon = weapon;
}

void Player::update()
{
	if (!is_alive())
		return;

	// Weapon equipment
	if (equipped_weapon)
		equipped_weapon->update();

	for(u32 i = 0; i < inventory->weapons.count(); ++i)
	{
		if (inventory->weapons[i] == nullptr)
			continue;

		if (input->key_pressed(weapon_equip_keys[i]))
			equip_weapon(inventory->weapons[i]);
	}

	update_movement();
	move(velocity * time_delta());

	// Blinking when hit!
	if (is_immune())
	{
		int pulse = time_elapsed_raw() * 10.f;
		mesh->set_disabled(pulse % 2);
	}
	else
	{
		mesh->set_disabled(false);
	}

	// Hide the drawable in case we're not in game-mode
	crosshair_line->set_disabled(!input->is_group_active(Input_Group::Game));
	crosshair_circle->set_disabled(!input->is_group_active(Input_Group::Game));
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
	if (input->key_pressed(Key::S) && !is_grounded)
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
	if (input->key_pressed(Key::LeftShift) && get_movement_input() != 0.f)
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
	if (input->key_pressed(Key::Spacebar))
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

	if (input->key_pressed(Key::Spacebar) && air_jumps > 0)
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
	float move_input = 0.f;
	if (input->key_down(Key::D))
		move_input += 1.f;
	if (input->key_down(Key::A))
		move_input -= 1.f;

	return move_input;
}

void Player::update_ui()
{
	Canvas_Style::alignment(Vec2(0.5f, 1.f));
	Canvas_Style::anchor(Vec2(0.5f, 1.f));
	Canvas_Style::position(Vec2(0.f, -8.f));

	ui->begin<WVertical_Box>();
	{
		Vertical_Box_Style::halign(Horizontal_Align::Center);

		// Inventory
		ui->begin<WHorizontal_Box>();
		{
			Horizontal_Box_Style::padding(4.f);
			for(u32 i = 0; i < inventory->weapons.count(); ++i)
			{
				ui->begin<WBox>(Vec2(32.f), Color(0.f, 0.f, 0.f, 0.4f));
				if (inventory->weapons[i] != nullptr)
				{
					Weapon_Type* type = inventory->weapons[i]->get_type();
					ui->add<WImage>(type->icon_path, Vec2(32.f));
				}

				ui->end();
			}
		}
		ui->end();

		// Health
		ui->begin<WHorizontal_Box>();
		{
			for(u32 i = 0; i < 3; ++i)
			{
				TString icon = (i < health) ? "texture/heart.tga" : "texture/heart_empty.tga";
				ui->add<WImage>(icon, Vec2(64.f));
			}
		}
		ui->end();
	}
	ui->end();
}

void Player::hit(const Vec3& direction)
{
	health -= 1;
	velocity *= 0.4f;
	velocity += direction * PLAYER_HIT_IMPULSE;

	immune_until = time_elapsed() + PLAYER_IMMUNE_TIME;

	if (!is_alive())
		mesh->set_disabled(true);
}