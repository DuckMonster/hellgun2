#include "meshresource.h"
#include "import/obj.h"
#include "import/msh.h"

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
	Msh_File msh;
	msh.load(get_absolute_path());
	msh.upload_to_mesh(&mesh, true);
}

void Mesh_Resource::free()
{
	mesh.free();
}