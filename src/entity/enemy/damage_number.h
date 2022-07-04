#pragma once
#include "gfx/grid_font.h"

#define DAMAGE_NMBR_IMPULSE (50.f)
#define DAMAGE_NMBR_FRICTION (5.5f)
#define DAMAGE_NMBR_GRAVITY (45.f)
#define DAMAGE_NMBR_DURATION (0.8f)

class Damage_Number
{
public:
	Damage_Number() {}

	void init();
	void begin(const Vec3& position, float damage, const Vec3& direction);
	void update();
	void render(const Render_Info& info);

	bool active = false;
	float begin_time;

	String damage_text;
	Vec3 position;
	Vec3 velocity;

	Grid_Font* font;
};