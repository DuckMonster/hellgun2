#include "pistol_ammo_drop.h"
#include "game/game.h"
#include "game/scene.h"
#include "resource/resource_common.h"
#include "resource/resource.h"
#include "player/player.h"
#include "pistol.h"

void Pistol_Ammo_Drop::update()
{
	velocity += -Vec3::up * 80.f * time_delta();

	// Calculate friction based on X-velocity
	float friction = 2.8f;
	velocity -= velocity * friction * time_delta();

	Sweep_Info info;
	info.object_mask = COBJ_World;

	Hit_Result hit = scene->sweep(Shape::aabb(position, Vec3(1.f)), velocity * time_delta(), info);
	if (hit.has_hit)
	{
		position = hit.position;
		velocity -= constrain_to_direction(velocity, hit.normal) * 1.5f;
	}
	else
	{
		position += velocity * time_delta();
	}

	// Check distance to player
	if (distance_sqrd(position, game->player->position) < Math::square(PISTOL_AMMO_PICKUP_RANGE))
	{
		// Pick up!
		host->ammo++;
		scene->destroy_entity(this);
	}
}

void Pistol_Ammo_Drop::render(const Render_Info& info)
{
	Material* mat = Resource::load_material("material/test.mat");

	mat->use();
	mat->set("u_ViewProjection", info.view_projection);
	mat->set("u_Model", mat_translation(position));

	Common_Mesh::rect.draw();
}