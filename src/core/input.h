#pragma once
#include "key.h"
#include "mouse.h"

struct Action_State
{
	bool state;
	u64 change_frame = (u64)-1;
};

enum class Input_Group : u8
{
	None,
	Game,
	Game_UI,
	Editor,
};

class Input
{
public:
	bool key_down(Key key);
	bool key_pressed(Key key);
	bool key_released(Key key);
	void set_key_state(Key key, bool state);

	bool mouse_down(Mouse_Btn btn);
	bool mouse_pressed(Mouse_Btn btn);
	bool mouse_released(Mouse_Btn btn);

	float mouse_x() { return _mouse_position.x; }
	float mouse_y() { return _mouse_position.y; }
	Vec2 mouse_position() { return _mouse_position; }
	float mouse_delta_x() { return mouse_delta().x; }
	float mouse_delta_y() { return mouse_delta().y; }
	Vec2 mouse_delta() { return _mouse_position - _previous_mouse_position; }
	i32 mouse_wheel_delta() { return _mouse_wheel_delta; }

	// Setting stuff
	// PROBABLY this should only be called from context
	void new_frame();

	void set_mouse_state(Mouse_Btn btn, bool state);
	void set_mouse_position(const Vec2& position) { _mouse_position = position; }

	void add_mouse_wheel(i32 delta) { _mouse_wheel_delta += delta; }
	void reset_mouse_wheel_delta() { _mouse_wheel_delta = 0; }

	// Group stuff
	void set_active_group(Input_Group group) { active_group = group; }
	bool is_group_active(Input_Group group) { return active_group == group; }
	bool should_ignore_input();

private:
	// Action stuff
	Action_State keyboard_states[(u32)Key::MAX];
	Action_State mouse_states[(u32)Mouse_Btn::MAX];

	// Mouse stuff
	Vec2 _previous_mouse_position;
	Vec2 _mouse_position;
	i32 _mouse_wheel_delta;

	// Group stuff
	Input_Group active_group = Input_Group::None;
};

extern Input* input;

// Use to change the current input query group
struct Input_Scope
{
	static Input_Scope* current;

	Input_Scope(Input_Group group) : group(group)
	{
		parent = current;
		current = this;
	}
	~Input_Scope()
	{
		current = parent;
	}

	Input_Group group;
	Input_Scope* parent;
};
#define INPUT_SCOPE(group) Input_Scope _scope##__LINE__(group)