#include "dummy.h"
#include "resource/resource_common.h"
#include "resource/resource.h"
#include "game/scene.h"

void Dummy::init()
{
	collider = scene->add_collider();
	collider->attach_shape(Shape::aabb(Vec3::zero, Vec3(3.f)));
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

	Shape shape = Shape::aabb(position, Vec3(3.f));

	Sweep_Info sweep_info;
	sweep_info.source_entity = this;
	sweep_info.ignore_self = true;

	Hit_Result hit = scene->sweep(shape, velocity * time_delta(), sweep_info);

	position = hit.position;
	collider->position = position;
}

void Dummy::render(const Render_Info& info)
{
	Material* mat = Resource::load_material("material/test.mat");

	mat->use();
	mat->set("u_ViewProjection", info.view_projection);
	mat->set("u_Model", mat_translation(position) * mat_scale(3.f));

	Common_Mesh::rect.draw();
}