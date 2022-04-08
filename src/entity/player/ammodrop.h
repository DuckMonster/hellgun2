#pragma once
#include "entity/entity.h"

struct Ammo_Drop : public Entity
{
	ENTITY_CLASS(Ammo_Drop)

public:
	Vec3 velocity;

	void update() override;
	void render(const Render_Info& info) override;
};