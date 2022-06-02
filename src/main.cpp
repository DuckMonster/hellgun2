#include <stdio.h>
#include "core/context.h"
#include "core/input.h"
#include "game/game.h"
#include "game/scene.h"
#include "game/editor.h"
#include "game/level.h"
#include "fx/fx.h"
#include "debug/debug.h"
#include "resource/resource.h"
#include "resource/resourcecommon.h"
#include "test.h"
#include "import/dat.h"
#include "import/obj.h"
#include "ui/ui.h"
#include "ui/ui_debug.h"
#include <stdlib.h>

int main()
{
	context.open("Hellgun", 800, 600);

	load_gl_extensions();

	load_common_resources();

	time_init();

	Level* lvl = Resource::load_level("level/test.lvl");

	debug = new Debug();
	debug->init();

	game = new Game();
	scene = new Scene();
	editor = new Editor();
	fx = new Fx();

	ui = new UI();
	ui_debug = new UI_Debug();
	ui_debug->init();

	game->init();
	editor->init();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	bool is_open = true;
	bool editor_mode = false;
	float next_hotreload_time = 0.f;

	while(context.is_open())
	{
		context.update();
		time_update();

		if (key_pressed(Key::Escape))
			context.close();

		// Toggle editor
		if (key_pressed(Key::Tab))
		{
			if (!editor_mode)
			{
				editor_mode = true;
				editor->enter_editor();
			}
			else
			{
				editor_mode = false;
				editor->exit_editor();
			}
		}

		if (next_hotreload_time < time_elapsed_raw())
		{
			Resource::update_hotreload();
			next_hotreload_time = time_elapsed_raw() + 1.f;
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Debug text
		debug->text(String::printf("%.02f ms", time_delta_raw() * 1000.f), Vec2(context.width, 0.f), Color::yellow, Color::yellow * 0.4f, Vec2(1.f, 0.f));
		debug->text(String::printf("Entity count: %d", scene->entities.count()), Vec2(context.width, 12.f), Color::yellow, Color::yellow * 0.4f, Vec2(1.f, 0.f));

		debug->text(String::printf("Num systems: %u", fx->systems.count()), Vec2(context.width, 24.f), Color::yellow, Color::yellow * 0.4f, Vec2(1.f, 0.f));

		if (editor_mode)
		{
			editor->update();
			editor->render();
		}
		else
		{
			game->update();
			game->render();
		}
	}

	delete game;
	delete scene;
	delete debug;
}