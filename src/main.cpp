#include "core/context.h"
#include "core/input.h"
#include "game/game.h"
#include "game/scene.h"
#include "game/editor.h"
#include "game/level.h"
#include "fx/fx.h"
#include "debug/debug.h"
#include "resource/resource.h"
#include "resource/resource_common.h"
#include "test.h"
#include "import/dat.h"
#include "import/obj.h"
#include "ui/ui.h"
#include "core/alloc/heap_allocator.h"
#include "core/alloc/temp_allocator.h"
#include <stdlib.h>
#include <stdio.h>

template<typename T, typename TAllocator>
struct Test_Base
{
};

template<typename T>
using Test = Test_Base<T, Heap_Allocator>;

int main()
{
	// Set thread priority
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	Temp_Allocator::init(50000);
	context.open("Hellgun", 1024, 768);

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
	ui->init();

	game->init();
	editor->init();

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);

	bool is_open = true;
	bool editor_mode = false;
	float next_hotreload_time = 0.f;

	while(context.is_open())
	{
		Heap_Allocator::new_frame();
		Temp_Allocator::new_frame();

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

/*
		if (next_hotreload_time < time_elapsed_raw())
		{
			Resource::update_hotreload();
			next_hotreload_time = time_elapsed_raw() + 1.f;
		}
		*/

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		// Debug text
		debug->text(TString::printf("%.02f ms", time_delta_raw() * 1000.f), Vec2(context.width, 0.f), Color::yellow, Color::yellow * 0.4f, Vec2(1.f, 0.f));
		debug->text(TString::printf("Entity count: %d", scene->entities.count()), Vec2(context.width, 12.f), Color::yellow, Color::yellow * 0.4f, Vec2(1.f, 0.f));
		debug->text(TString::printf("Num systems: %u", fx->systems.count()), Vec2(context.width, 24.f), Color::yellow, Color::yellow * 0.4f, Vec2(1.f, 0.f));

		// Heap
		debug->text(TString::printf("Heap count: %u", Heap_Allocator::total_allocation_count), Vec2(context.width, 44.f), Color::white, Color::red * 0.4f, Vec2(1.f, 0.f));
		debug->text(TString::printf("Heap size: %u B", Heap_Allocator::total_allocation_size), Vec2(context.width, 54.f), Color::white, Color::red * 0.4f, Vec2(1.f, 0.f));
		debug->text(TString::printf("This Frame: %u", Heap_Allocator::frame_allocation_count), Vec2(context.width, 64.f), Color::white, Color::red * 0.4f, Vec2(1.f, 0.f));

		// Temp
		debug->text(TString::printf("Temp capacity: %u B", Temp_Allocator::buffer_size), Vec2(context.width, 84.f), Color::white, Color::blue * 0.4f, Vec2(1.f, 0.f));
		debug->text(TString::printf("Temp usage: %u B (%u)", Temp_Allocator::frame_allocation_size, Temp_Allocator::frame_allocation_count), Vec2(context.width, 94.f), Color::white, Color::blue * 0.4f, Vec2(1.f, 0.f));

		if (time_delta() > .005f)
			debug->print(TString::printf("Hitch: %fms", time_delta() * 1000.f), 2.f);

		// Render debug
		Render_Info info;
		info.ui_canvas = mat_ortho(0.f, context.width, context.height, 0.f);

		if (editor_mode)
		{
			info.view = editor->camera.get_view();
			info.projection = editor->camera.get_projection();
		}
		else
		{
			info.view = game->camera.get_view();
			info.projection = game->camera.get_projection();
		}
		info.view_projection = info.projection * info.view;

		debug->render(info);
		debug->new_frame();
	}

	delete game;
	delete scene;
	delete debug;
}