#include "pistolprojectile.h"
#include "resource/resource.h"
#include "resource/resourcecommon.h"
#include "math/random.h"
#include "game/scene.h"
#include "fx/fx.h"
#include "fx/trailsystem.h"
#include "fx/surfaceimpactsystem.h"
#include "pistolammodrop.h"

void Pistol_Projectile::init()
{
	Trail_Params params;
	params.size = 0.4f;
	params.frequency = 100.f;
	params.color = Color::white;

	trail = fx->spawn_system<Trail_System>(position, params);
	trail->attach_to(this);
}

void Pistol_Projectile::on_destroyed()
{
	trail->detach();
	trail->finish_system();
}

void Pistol_Projectile::update()
{
	Hit_Result hit = scene->sweep_aabb(AABB::from_center_size(position, Vec3(0.5f)), velocity * time_delta());
	if (hit.has_hit)
	{
		scene->destroy_entity(this);
		if (hit.collider->owner)
			scene->destroy_entity(hit.collider->owner);

		// Spawn ammo drop
		Vec3 drop_velocity = normalize(velocity - constrain_to_direction(velocity, hit.normal) * 1.5f);
		drop_velocity *= Random::range(50.f, 100.f);

		Pistol_Ammo_Drop* drop = scene->spawn_entity<Pistol_Ammo_Drop>(hit.position + hit.normal * 0.5f);
		drop->velocity = drop_velocity;
		drop->host = host;

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

void Pistol_Projectile::render(const Render_Info& info)
{
	Material* mat = Resource::load_material("material/test.mat");

	mat->use();
	mat->set("u_ViewProjection", info.view_projection);
	mat->set("u_Model", mat_translation(position) * mat_scale(0.5f));

	Common_Mesh::rect.draw();
}