#include "time.h"
#include <windows.h>

namespace
{
	u64 clk_freq;
	u64 clk_begin;

	u64 clk_prev;

	float calc_delta_between(u64 from, u64 to)
	{
		u64 delta = to - from;
		return delta / (float)clk_freq;
	}

	float frame_delta;
	float frame_elapsed;

	float time_dilation = 1.f;
}

void time_init()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&clk_freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&clk_begin);
	QueryPerformanceCounter((LARGE_INTEGER*)&clk_prev);
}

void time_update()
{
	u64 clk_now;
	QueryPerformanceCounter((LARGE_INTEGER*)&clk_now);

	frame_delta = calc_delta_between(clk_prev, clk_now);
	frame_elapsed = calc_delta_between(clk_begin, clk_now);

	clk_prev = clk_now;
}

float time_delta()
{
	return frame_delta * time_dilation;
}
float time_delta_raw()
{
	return frame_delta;
}
float time_elapsed()
{
	return frame_elapsed;
}

void time_dilate(float value)
{
	time_dilation = value;
}