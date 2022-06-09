#pragma once
#include "collision/aabb.h"
#include "container/array.h"
#include "gfx/mesh.h"
#include "gfx/render_info.h"

struct Material;
class Collider;

struct Level
{
	struct Prop
	{
		bool render = false;
		bool collision = false;

		AABB bounds;
		Mesh mesh;
		Material* material;
		Mat4 transform;
		Collider* collider;
	};

	Array<Prop> props;
	Array<AABB> colliders;
	bool is_open = false;

	void open();
	void close();

	void clear();

	void render(const Render_Info& info);
};
extern Level* level;