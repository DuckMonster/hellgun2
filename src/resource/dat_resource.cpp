#include "dat_resource.h"

void Dat_Resource::load()
{
	dat.load_file(path);
}

void Dat_Resource::free()
{
	dat.free();
}