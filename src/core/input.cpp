#include "input.h"
#include "engine.h"

Input* input;
Input_Scope* Input_Scope::current = nullptr;

void Input::new_frame()
{
	_previous_mouse_position = _mouse_position;
}

bool Input::key_down(Key key)
{
	if (should_ignore_input())
		return false;

	return keyboard_states[(u32)key].state; 
}
bool Input::key_pressed(Key key)
{
	if (should_ignore_input())
		return false;

	return keyboard_states[(u32)key].state && keyboard_states[(u32)key].change_frame == frame_num;
}
bool Input::key_released(Key key)
{
	if (should_ignore_input())
		return false;

	return !keyboard_states[(u32)key].state && keyboard_states[(u32)key].change_frame == frame_num;
}

void Input::set_key_state(Key key, bool state)
{
	keyboard_states[(u32)key].state = state;
	keyboard_states[(u32)key].change_frame = frame_num;
}

bool Input::mouse_down(Mouse_Btn btn)
{
	if (should_ignore_input())
		return false;

	return mouse_states[(u32)btn].state;
}
bool Input::mouse_pressed(Mouse_Btn btn)
{
	if (should_ignore_input())
		return false;

	return mouse_states[(u32)btn].state && mouse_states[(u32)btn].change_frame == frame_num;
}
bool Input::mouse_released(Mouse_Btn btn)
{
	if (should_ignore_input())
		return false;

	return !mouse_states[(u32)btn].state && mouse_states[(u32)btn].change_frame == frame_num;
}

void Input::set_mouse_state(Mouse_Btn btn, bool state)
{
	mouse_states[(u32)btn].state = state;
	mouse_states[(u32)btn].change_frame = frame_num;
}

bool Input::should_ignore_input()
{
	if (Input_Scope::current == nullptr)
		return false;

	return Input_Scope::current->group != active_group;
}