#include "ammodrop.h"
#include "game/scene.h"
#include "resource/resourcecommon.h"

void Ammo_Drop::update()
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

void Ammo_Drop::render(const Render_Info& info)
{
	Common_Mat::test.use();
	Common_Mat::test.set("u_ViewProjection", info.view_projection);
	Common_Mat::test.set("u_Model", mat_translation(position) * mat_scale(1.f));

	Common_Mesh::rect.draw();
}