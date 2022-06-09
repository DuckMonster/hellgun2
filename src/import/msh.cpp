#include "msh.h"
#include "gfx/mesh.h"
#include "core/parse/binary_stream.h"

void Msh_File::load(const String& path)
{
	Binary_Stream stream;
	if (!stream.open(path))
		return;

	// Read transform
	stream.read<Mat4>(&transform);

	// Read vertices
	u32 num_vertices = stream.read<u32>();
	positions.reserve(num_vertices);

	for(u32 i = 0; i < num_vertices; ++i)
		positions.add(stream.read<Vec3>());

	// Read UVs
	u32 num_uvs = stream.read<u32>();
	uvs.reserve(num_uvs);

	for(u32 i = 0; i < num_uvs; ++i)
		uvs.add(stream.read<Vec2>());

	// Polygons
	u32 num_polygons = stream.read<u32>();
	for(u32 i = 0; i < num_polygons; ++i)
	{
		Polygon& poly = polygons.add_default();
		poly.normal = stream.read<Vec3>();

		// Number of vertices on the polygon
		u32 vert_num = stream.read<u32>();
		poly.vertex_num = vert_num;
		poly.position_idx.reserve(vert_num);
		poly.uv_idx.reserve(vert_num);

		for(u32 vert_idx = 0; vert_idx < vert_num; ++vert_idx)
		{
			poly.position_idx.add(stream.read<u32>());
			poly.uv_idx.add(stream.read<u32>());
		}
	}
}

void Msh_File::upload_to_mesh(Mesh* mesh, bool apply_transform)
{
	struct Vertex
	{
		Vec3 position;
		Vec2 uv;
		Vec3 normal;
	};

	Array<Vertex> vertices;

	// At least 3 vertices per polygon, potentially more
	vertices.reserve(polygons.count() * 3);
	auto push_vertex = [&](const Polygon& poly, u32 idx)
	{
		Vertex& vert = vertices.add_uninitialized();
		vert.position = positions[poly.position_idx[idx]];
		if (apply_transform)
			vert.position = transform * vert.position;

		vert.uv = uvs[poly.uv_idx[idx]];
		vert.normal = poly.normal;
	};

	for(const auto& poly : polygons)
	{
		// Triangulate each polygon
		for(u32 idx = 2; idx < poly.vertex_num; ++idx)
		{
			push_vertex(poly, 0);
			push_vertex(poly, idx - 1);
			push_vertex(poly, idx);
		}
	}

	// Upload
	mesh->buffer_data(0, vertices.count() * sizeof(Vertex), vertices.data());
	mesh->bind_attribute(0, 0, 3, sizeof(Vertex), offsetof(Vertex, position));
	mesh->bind_attribute(0, 1, 2, sizeof(Vertex), offsetof(Vertex, uv));
	mesh->bind_attribute(0, 2, 3, sizeof(Vertex), offsetof(Vertex, normal));

	mesh->draw_mode = GL_TRIANGLES;
	mesh->draw_offset = 0;
	mesh->draw_num = vertices.count();
}

AABB Msh_File::calculate_bounds(bool apply_transform)
{
	Vec3 min = Vec3(BIG_NUMBER);
	Vec3 max = Vec3(-BIG_NUMBER);

	for(auto pos : positions)
	{
		if (apply_transform)
			pos = transform * pos;

		min = component_min(pos, min);
		max = component_max(pos, max);
	}

	return AABB(min, max);
}