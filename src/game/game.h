#pragma once
#include "camera.h"
#include "math/ray.h"

#define EDITOR_CAM_SPD (15.f)
#define EDITOR_CAM_SENS (0.001f)

class Player;
struct Game
{
	Camera camera;
	Camera editor_camera;

	Player* player;
	float enemy_spawn_time = 0.f;

	void init();
	void update();
	void render();

	Ray get_mouse_world_ray();

	// EDITOR STUFF
	bool editor_mode = false;
	i32 time_dilation_exp = 0;

private:
	void editor_update();
	void change_time_dilation(i32 delta);
};

extern Game* game;