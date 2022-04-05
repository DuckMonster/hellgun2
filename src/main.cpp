#include <stdio.h>
#include "core/context.h"
#include "core/input.h"
#include "game/game.h"
#include "game/player.h"
#include "debug/debug.h"
#include "container/array.h"

int main()
{
	context.open("Hellgun", 800, 600);

	load_gl_extensions();
	time_init();

	debug.init();
	game.init();

	glEnable(GL_DEPTH_TEST);

	bool is_open = true;
	while(context.is_open())
	{
		context.update();
		time_update();

		if (key_pressed(Key::Escape))
			context.close();

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game.update();
		game.render();
	}
}