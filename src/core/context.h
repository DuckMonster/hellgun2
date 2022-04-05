#pragma once

struct Context
{
	void open(const char* title, u32 in_width, u32 in_height);
	bool is_open();

	void close();

	void update();

	void lock_cursor();
	void unlock_cursor();

	u32 width;
	u32 height;
};
extern Context context;