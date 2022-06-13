#include "drawable.h"
#include "entity/entity.h"
#include "resource/resource.h"
#include "gfx/mesh.h"
#include "gfx/material.h"
#include "gfx/texture.h"

void Drawable::load(const String& mesh_path, const String& material_path, const String& texture_path)
{
	mesh = Resource::load_mesh(mesh_path);
	material = Resource::load_material(material_path);

	if (!texture_path.is_null())
		texture = Resource::load_texture(texture_path);
}

void Drawable::render(const Render_Info& info)
{
	if (flags & DRW_Disabled)
		return;

	glEnable(GL_POLYGON_OFFSET_FILL);

	if (flags & DRW_Blend)
		glEnable(GL_BLEND);

	material->use();

	material->set("u_Model", get_world_matrix());

	material->set("u_ViewProjection", info.view_projection);
	if (flags & DRW_UI)
		material->set("u_ViewProjection", info.ui_canvas);

	if (texture)
		texture->bind();

	on_render.execute(info);

	glPolygonOffset(depth_offset, 0.f);
	mesh->draw();

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_BLEND);
}

Mat4 Drawable::get_world_matrix()
{
	if (attach_entity)
		return mat_translation(attach_entity->position) * attach_entity->rotation.matrix() * matrix;

	return matrix;
}