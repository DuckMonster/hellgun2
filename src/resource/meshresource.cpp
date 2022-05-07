#include "meshresource.h"
#include "import/obj.h"

void Mesh_Resource::init()
{
	u32 vertex_size = sizeof(Obj_File::Vertex);

	mesh.init();
	mesh.add_buffer(0);
	mesh.bind_attribute(0, 0, 3, vertex_size, offsetof(Obj_File::Vertex, position));
	mesh.bind_attribute(0, 1, 2, vertex_size, offsetof(Obj_File::Vertex, uv));
	mesh.bind_attribute(0, 2, 3, vertex_size, offsetof(Obj_File::Vertex, normal));
}

void Mesh_Resource::load()
{
	Obj_File obj;
	obj.load(path);

	mesh.buffer_data(0, obj.vertices.count() * sizeof(Obj_File::Vertex), obj.vertices.data());

	mesh.draw_mode = GL_TRIANGLES;
	mesh.draw_offset = 0;
	mesh.draw_num = obj.vertices.count();
}

void Mesh_Resource::free()
{
	mesh.free();
}