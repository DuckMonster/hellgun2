#include "damage_number.h"
#include "resource/resource.h"

void Damage_Number::init()
{
	font = Resource::load_font("ui/font.dat");
}

void Damage_Number::begin(const Vec3& position, float damage, const Vec3& direction)
{
	active = true;

	this->position = position;
	this->velocity = normalize(direction + Vec3(0.f, 1.f, 0.f)) * DAMAGE_NMBR_IMPULSE;

	damage_text = String::printf("%d", int(damage));
	begin_time = time_elapsed();
}

void Damage_Number::update()
{
	if (time_since(begin_time) > DAMAGE_NMBR_DURATION)
	{
		active = false;
		return;
	}

	velocity -= Vec3::up * DAMAGE_NMBR_GRAVITY * time_delta();
	velocity -= velocity * DAMAGE_NMBR_FRICTION * time_delta();

	position += velocity * time_delta();
}

void Damage_Number::render(const Render_Info& info)
{
	Vec2 canvas_location = info.world_to_canvas(position);

	Grid_Font_Info font_info;
	font_info.foreground = Color::red;
	font_info.background = Color::transparent;
	font_info.alignment = Vec2(0.5f, 1.f);
	font_info.scale = 2.f;

	font->render_text(damage_text, canvas_location, font_info, info);
}