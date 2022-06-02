#include "levelresource.h"
#include "core/io.h"
#include "import/msh.h"
#include "core/parse/binarystream.h"

void Level_Resource::init()
{
}

void Level_Resource::load()
{
	level.clear();

	// Chop off the extension and add a trailing slash
	String prop_root = get_absolute_path().right_chop(4) + "/";

	Binary_Stream stream;
	if (!stream.open(get_absolute_path()))
		return;

	u32 num_props = stream.read<u32>();
	for(u32 prop_idx = 0; prop_idx < num_props; ++prop_idx)
	{
		Level::Prop& prop = level.props.add_default();
		String name = stream.read_str();

		// Fetch render/collision options and prop name
		prop.render = stream.read<bool>();
		prop.collision = stream.read<bool>();

		String material_path = stream.read_str();

		// Load mesh file
		Msh_File msh;
		msh.load(prop_root + name + ".msh");

		// Write meshes, materials, etc
		prop.mesh.init();
		prop.mesh.add_buffer(0);
		msh.upload_to_mesh(&prop.mesh, false);
		prop.material = Resource::load_material(material_path);
		prop.transform = msh.transform;
		prop.bounds = msh.calculate_bounds();
	}
}

void Level_Resource::free()
{

}