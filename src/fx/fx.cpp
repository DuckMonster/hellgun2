#include "fx.h"
#include "particlesystem.h"
#include "debug/debug.h"

Fx* fx;

void Fx::update()
{
	for(u32 i = 0; i < systems.count(); ++i)
	{
		systems[i]->update();
		if (systems[i]->should_be_destroyed())
		{
			delete systems[i];
			systems.remove_at_swap(i);
			i--;
		}
	}

	debug->print(String::printf("Num systems: %u", systems.count()));
}

void Fx::render(const Render_Info& info)
{
	for(auto* system : systems)
		system->render(info);
}