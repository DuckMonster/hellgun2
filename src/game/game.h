#pragma once
#include "camera.h"

#define EDITOR_CAM_SPD (15.f)
#define EDITOR_CAM_SENS (0.001f)

struct Game
{
	Camera camera;
	Camera editor_camera;

	void init();
	void update();
	void render();

	bool editor_mode = false;
	i32 time_dilation_exp = 0;

private:
	void editor_update();
	void change_time_dilation(i32 delta);
};

extern Game game;