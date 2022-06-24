#pragma once

struct UI_Rect
{
	static UI_Rect zero;
	Vec2 position;
	Vec2 size;

	UI_Rect() {}
	UI_Rect(const Vec2& position, const Vec2& size)
		:position(position), size(size) {}
	UI_Rect(float x, float y, float width, float height)
		:position(x, y), size(width, height) {}
};