#include "enemydamagesystem.h"
#include "resource/resource.h"
#include "gfx/material.h"
#include "gfx/mesh.h"

void Enemy_Damage_System::init(Mesh* mesh, const Mat4& transform)
{
	this->mesh = mesh;
	this->material = Resource::load_material("material/fx/enemy_damage.mat");
	this->transform = transform;
}

void Enemy_Damage_System::system_update()
{
	if (get_age() > 0.4f)
		finish_system();
}

void Enemy_Damage_System::system_render(const Render_Info& info)
{
	material->use();
	material->set("u_ViewProjection", info.view_projection);
	material->set("u_Model", transform);

	mesh->bind();
	mesh->draw();
}