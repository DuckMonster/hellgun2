#pragma once
#include "entity/entity.h"

class Collider;

class Dummy : public Entity
{
	ENTITY_CLASS(Dummy)

public:
	Vec3 velocity = Vec3::zero;
	Collider* collider;

	void init() override;
	void on_destroyed() override;

	void update() override;
	void render(const Render_Info& info) override;
};