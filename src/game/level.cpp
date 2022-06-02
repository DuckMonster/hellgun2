#include "level.h"
#include "gfx/mesh.h"
#include "gfx/material.h"
#include "scene.h"

Level* level = nullptr;

void Level::open()
{
	if (is_open)
		return;

	for(Prop& prop : props)
	{
		if (prop.collision)
		{
			prop.collider = scene->add_collider();
			prop.collider->attach_shape(Shape::aabb(prop.bounds.center(), prop.bounds.size()));
			prop.collider->object_type = COBJ_World;
		}
	}
}

void Level::close()
{
	if (!is_open)
		return;
}

void Level::clear()
{
	for(Prop& prop : props)
	{
		prop.mesh.free();
		if (prop.collider)
			scene->destroy_collider(prop.collider);
	}

	props.empty();
	colliders.empty();
}

void Level::render(const Render_Info& info)
{
	for(const Prop& prop : props)
	{
		if (!prop.render)
			continue;

		prop.material->use();
		prop.material->set("u_Model", prop.transform);
		prop.material->set("u_ViewProjection", info.view_projection);

		prop.mesh.draw();
	}
}