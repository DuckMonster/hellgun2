#include "input.h"
#include "engine.h"
static Key_State keyboard[(u32)Key::MAX];
static Key_State mouse[(u32)Mouse_Btn::MAX];

static float _mouse_x;
static float _mouse_y;
static float _mouse_delta_x;
static float _mouse_delta_y;
static i32 _mouse_wheel_delta = 0;

bool key_down(Key key)
{
	return keyboard[(u32)key].state; 
}
bool key_pressed(Key key)
{
	return keyboard[(u32)key].state && keyboard[(u32)key].change_frame == frame_num;
}
bool key_released(Key key)
{
	return !keyboard[(u32)key].state && keyboard[(u32)key].change_frame == frame_num;
}

void set_key_state(Key key, bool state)
{
	keyboard[(u32)key].state = state;
	keyboard[(u32)key].change_frame = frame_num;
}

bool mouse_down(Mouse_Btn btn)
{
	return mouse[(u32)btn].state;
}
bool mouse_pressed(Mouse_Btn btn)
{
	return mouse[(u32)btn].state && mouse[(u32)btn].change_frame == frame_num;
}
bool mouse_released(Mouse_Btn btn)
{
	return !mouse[(u32)btn].state && mouse[(u32)btn].change_frame == frame_num;
}

float mouse_x() { return _mouse_x; }
float mouse_y() { return _mouse_y; }
float mouse_delta_x() { return _mouse_delta_x; }
float mouse_delta_y() { return _mouse_delta_y; }
i32 mouse_wheel_delta() { return _mouse_wheel_delta; }

void set_mouse_state(Mouse_Btn btn, bool state)
{
	mouse[(u32)btn].state = state;
	mouse[(u32)btn].change_frame = frame_num;
}

void set_mouse_x(float x)
{
	_mouse_delta_x = x - _mouse_x;
	_mouse_x = x;
}
void set_mouse_y(float y)
{
	_mouse_delta_y = y - _mouse_y;
	_mouse_y = y;
}

void reset_mouse_delta()
{
	_mouse_delta_x = _mouse_delta_y = 0.f;
}

void add_mouse_wheel(i32 delta)
{
	_mouse_wheel_delta += delta;
}

void reset_mouse_wheel_delta()
{
	_mouse_wheel_delta = 0;
}