#pragma once
#include "camera.h"

#define EDITOR_CAM_SPD (15.f)
#define EDITOR_CAM_SENS (0.001f)

class Editor
{
public:
	bool active = false;

	Camera camera;
	i32 time_dilation_exp = 0;

	// Debug sweeping stuff
	bool has_sweep = false;
	Vec3 sweep_origin;
	Vec3 sweep_direction;
	float sweep_length = 50.f;

	void init();
	void update();
	void render();

	void enter_editor();
	void exit_editor();

private:
	void change_time_dilation(i32 delta);
};

extern Editor* editor;