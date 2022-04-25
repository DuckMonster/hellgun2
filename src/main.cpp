#include <stdio.h>
#include "core/context.h"
#include "core/input.h"
#include "game/game.h"
#include "game/scene.h"
#include "fx/fx.h"
#include "debug/debug.h"
#include "resource/resource.h"
#include "resource/resourcecommon.h"
#include "test.h"
#include "import/dat.h"

int main()
{
	context.open("Hellgun", 800, 600);

	load_gl_extensions();

	load_common_resources();

	time_init();

	debug = new Debug();
	debug->init();

	game = new Game();
	scene = new Scene();
	fx = new Fx();

	game->init();

	glEnable(GL_DEPTH_TEST);

	bool is_open = true;
	float next_hotreload_time = 0.f;

	while(context.is_open())
	{
		context.update();
		time_update();

		if (key_pressed(Key::Escape))
			context.close();

		if (next_hotreload_time < time_elapsed_raw())
		{
			Resource::update_hotreload();
			next_hotreload_time = time_elapsed_raw() + 1.f;
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game->update();
		game->render();
	}

	delete game;
	delete scene;
	delete debug;
}