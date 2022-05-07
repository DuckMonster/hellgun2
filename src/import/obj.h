#pragma once
#include "container/array.h"

struct Obj_File
{
	struct Vertex
	{
		Vec3 position;
		Vec2 uv;
		Vec3 normal;

		Vertex(const Vec3& position, const Vec2& uv, const Vec3& normal)
			:position(position), uv(uv), normal(normal) {}
	}; 

	Array<Vertex> vertices;

	void load(const String& path);
};