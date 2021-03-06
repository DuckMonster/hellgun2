#include "time.h"
#include <windows.h>

constexpr float MAX_DELTA_TIME = 0.1f;

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

	frame_delta = frame_elapsed = 0.f;
}

void time_update()
{
	u64 clk_now;
	QueryPerformanceCounter((LARGE_INTEGER*)&clk_now);

	frame_delta = calc_delta_between(clk_prev, clk_now);
	frame_elapsed += frame_delta * time_dilation;

	clk_prev = clk_now;

	if (frame_delta > MAX_DELTA_TIME)
		frame_delta = MAX_DELTA_TIME;
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
float time_elapsed_raw()
{
	return calc_delta_between(clk_begin, clk_prev);
}

float time_since(float time_point)
{
	return time_elapsed() - time_point;
}
bool time_has_reached(float time_point)
{
	return time_elapsed() >= time_point;
}

void time_dilate(float value)
{
	time_dilation = value;
}

Stop_Watch::Stop_Watch()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&start_time);
}

float Stop_Watch::duration()
{
	u64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);

	return calc_delta_between(now, start_time);
}

u64 Stop_Watch::duration_ms()
{
	u64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);

	u64 delta = now - start_time;
	return (delta * 1000) / clk_freq;
}