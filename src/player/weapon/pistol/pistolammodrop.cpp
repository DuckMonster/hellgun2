#include "pistolammodrop.h"
#include "game/scene.h"
#include "resource/resourcecommon.h"
#include "resource/resource.h"

void Pistol_Ammo_Drop::update()
{
	velocity += -Vec3::up * 40.f * time_delta();

	// Calculate friction based on X-velocity
	float friction = 0.7f;
	friction += Math::abs(velocity.x) * 0.05f;

	velocity -= velocity * friction * time_delta();

	Hit_Result hit = scene->sweep_aabb(AABB::from_center_size(position, Vec3(1.f)), velocity * time_delta());
	if (hit.has_hit)
	{
		position = hit.position;
		velocity -= constrain_to_direction(velocity, hit.normal) * 1.5f;
	}
	else
	{
		position += velocity * time_delta();
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