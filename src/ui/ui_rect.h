#pragma once

struct UI_Rect
{
	Vec2 position;
	Vec2 size;

	UI_Rect() {}
	UI_Rect(const Vec2& position, const Vec2& size)
		:position(position), size(size) {}
};