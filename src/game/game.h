#pragma once
#include "camera.h"
#include "math/ray.h"

class Player;
struct Game
{
	Camera camera;
	Camera editor_camera;

	Player* player;

	bool spawn_enemies = false;
	float next_spawn_time = 0.f;

	void init();
	void update();
	void render();

	Ray get_mouse_world_ray();
	Vec3 get_mouse_game_position();
	Vec2 world_to_canvas(const Vec3& world);
};

extern Game* game;