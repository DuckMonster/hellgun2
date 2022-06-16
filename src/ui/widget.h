#pragma once

class Widget
{
public:
	Vec2 get_desired_size() { return Vec2::zero; }
	void render(const Render_Info& info);
};