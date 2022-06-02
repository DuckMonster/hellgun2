#pragma once
#include "container/array.h"
#include "collision/aabb.h"
struct Mesh;

struct Msh_File
{
	struct Polygon
	{
		Vec3 normal;

		u32 vertex_num;
		Array<u32> position_idx;
		Array<u32> uv_idx;
	};

	Mat4 transform;

	Array<Vec3> positions;
	Array<Vec2> uvs;
	Array<Polygon> polygons;

	void load(const String& path);
	void upload_to_mesh(Mesh* mesh, bool apply_transform = true);
	AABB calculate_bounds(bool apply_transform = true);
};