#pragma once
#include "key.h"
#include "mouse.h"

struct Key_State
{
	bool state;
	u64 change_frame = (u64)-1;
};

bool key_down(Key key);
bool key_pressed(Key key);
bool key_released(Key key);
void set_key_state(Key key, bool state);

bool mouse_down(Mouse_Btn btn);
bool mouse_pressed(Mouse_Btn btn);
bool mouse_released(Mouse_Btn btn);

float mouse_x();
float mouse_y();
float mouse_delta_x();
float mouse_delta_y();
i32 mouse_wheel_delta();

void set_mouse_state(Mouse_Btn btn, bool state);
void set_mouse_x(float x);
void set_mouse_y(float y);
void reset_mouse_delta();

void add_mouse_wheel(i32 delta);
void reset_mouse_wheel_delta();