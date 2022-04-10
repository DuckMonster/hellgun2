#include "dummy.h"
#include "resource/resourcecommon.h"
#include "game/scene.h"

void Dummy::init()
{
	collider = scene->add_collider();
	collider->set_aabb(Vec3(3.f));
	collider->owner = this;
	collider->position = position;
}

void Dummy::on_destroyed()
{
	scene->destroy_collider(collider);
}

void Dummy::update()
{
	velocity += -Vec3::up * 5.f * time_delta();

	AABB aabb = AABB::from_center_size(position, Vec3(3.f));

	Sweep_Info sweep_info;
	sweep_info.source_entity = this;
	sweep_info.ignore_self = true;

	Hit_Result hit = scene->sweep_aabb(aabb, velocity * time_delta(), sweep_info);

	position = hit.position;
	collider->position = position;
}

void Dummy::render(const Render_Info& info)
{
	Common_Mat::test.use();
	Common_Mat::test.set("u_ViewProjection", info.view_projection);
	Common_Mat::test.set("u_Model", mat_translation(position) * mat_scale(3.f));

	Common_Mesh::rect.draw();
}