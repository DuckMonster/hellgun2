#pragma once

struct UI_Rect
{
	Vec2 min;
	Vec2 max;

	UI_Rect() {}
	UI_Rect(const Vec2& min, const Vec2& max)
		: min(min), max(max) {}

	Vec2 size() { return max - min; }
	Vec2 center() { return (max + min) * 0.5f; }
};