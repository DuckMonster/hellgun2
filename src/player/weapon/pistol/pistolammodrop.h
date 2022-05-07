#pragma once
#include "entity/entity.h"

#define PISTOL_AMMO_PICKUP_RANGE (1.5f)

class Pistol;

struct Pistol_Ammo_Drop : public Entity
{
	ENTITY_CLASS(Pistol_Ammo_Drop)

public:
	Pistol* host;
	Vec3 velocity;

	void update() override;
	void render(const Render_Info& info) override;
};