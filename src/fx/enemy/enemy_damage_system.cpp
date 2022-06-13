#include "enemy_damage_system.h"
#include "resource/resource.h"
#include "gfx/material.h"
#include "gfx/mesh.h"
#include "gfx/texture.h"
#include "game/drawable.h"

static const float DURATION = 0.11f;

void Enemy_Damage_System::init(Drawable* drawable)
{
	this->mesh = drawable->mesh;
	this->texture = drawable->texture;
	this->material = Resource::load_material("material/fx/enemy_damage.mat");
	this->transform = drawable->get_world_matrix();
}

void Enemy_Damage_System::init(Mesh* mesh, Texture* texture, const Mat4& transform)
{
	this->mesh = mesh;
	this->texture = texture;
	this->material = Resource::load_material("material/fx/enemy_damage.mat");
	this->transform = transform;
}

void Enemy_Damage_System::system_update()
{
	if (get_age() > DURATION)
		finish_system();
}

void Enemy_Damage_System::system_render(const Render_Info& info)
{
	material->use();
	material->set("u_ViewProjection", info.view_projection);
	material->set("u_Model", transform);
	material->set("u_NormalizedAge", get_age() / DURATION);

	texture->bind();

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	mesh->bind();
	mesh->draw();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}