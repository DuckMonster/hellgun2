#include "bullet.h"
#include "resource/resourcecommon.h"
#include "game/scene.h"
#include "ammodrop.h"
#include "fx/fx.h"
#include "fx/trailsystem.h"
#include "fx/surfaceimpactsystem.h"

void Bullet::init()
{
	Trail_Params params;
	params.size = 0.4f;
	params.frequency = 100.f;
	params.color = Color::white;

	trail = fx->spawn_system<Trail_System>(position, params);
	trail->attach_to(this);
}

void Bullet::on_destroyed()
{
	trail->detach();
	trail->finish_system();
}

void Bullet::update()
{
	Hit_Result hit = scene->sweep_aabb(AABB::from_center_size(position, Vec3(0.5f)), velocity * time_delta());
	if (hit.has_hit)
	{
		scene->destroy_entity(this);
		if (hit.collider->owner)
			scene->destroy_entity(hit.collider->owner);

		// Spawn ammo drop
		Vec3 drop_velocity = velocity - constrain_to_direction(velocity, hit.normal) * 1.5f;
		drop_velocity *= 0.5f;

		Ammo_Drop* drop = scene->spawn_entity<Ammo_Drop>(hit.position + hit.normal * 0.5f);
		drop->velocity = drop_velocity;

		// Spawn FX
		fx->spawn_system<Surface_Impact_System>(hit.position, hit.normal, normalize(velocity), normalize(drop_velocity));
	}
	else
	{
		position += velocity * time_delta();

		if (get_age() > 2.f)
			scene->destroy_entity(this);
	}
}

void Bullet::render(const Render_Info& info)
{
	Common_Mat::test.use();
	Common_Mat::test.set("u_ViewProjection", info.view_projection);
	Common_Mat::test.set("u_Model", mat_translation(position) * mat_scale(0.5f));

	Common_Mesh::rect.draw();
}