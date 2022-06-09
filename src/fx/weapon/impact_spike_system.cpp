#include "impact_spike_system.h"
#include "gfx/mesh.h"
#include "gfx/material.h"
#include "resource/resource.h"

void Impact_Spike_System::init(float scale, float duration, const Vec3& direction)
{
	this->scale = scale;
	this->duration = duration;

	// Offset the spike backwards, since the mesh is offset forwards
	velocity = direction * 120.f;
	position -= direction * 3.f;
}

void Impact_Spike_System::system_update()
{
	if (get_age() > duration)
		finish_system();

	velocity -= velocity * 22.5f * time_delta();
	position += velocity * time_delta();
}

void Impact_Spike_System::system_render(const Render_Info& info)
{
	Mesh* mesh = Resource::load_mesh("mesh/spike.msh");
	Material* material = Resource::load_material("material/fx/spike.mat");

	Mat4 model = mat_translation(position) * mat_orient_x(velocity);

	material->use();
	material->set("u_Model", model);
	material->set("u_ViewProjection", info.view_projection);
	material->set("u_Scale", scale);
	material->set("u_NormalizedAge", get_age() / duration);

	glDisable(GL_DEPTH_TEST);
	mesh->draw();
	glEnable(GL_DEPTH_TEST);
}