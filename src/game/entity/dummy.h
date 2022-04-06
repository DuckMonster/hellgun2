#pragma once
#include "entity.h"

class Collider;

class Dummy : public Entity
{
public:
	Vec3 velocity = Vec3::zero;
	Collider* collider;

	void init() override;
	void update() override;
	void render(const Render_Info& info) override;
};